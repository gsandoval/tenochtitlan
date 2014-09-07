#include "server/socketserver.h"

#include <sstream>
#include <thread>
#include <chrono>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		SocketServer::SocketServer() : processing(false), stopped(false), max_worker_count(10)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("SocketServer"));
		}

		SocketServer::~SocketServer()
		{
			Stop();
			logger->Debug(__func__, "~SocketServer");
		}

		void SocketServer::HandleNewConnection(shared_ptr<socket::TcpClientConnection> connection)
		{
			unique_lock<mutex> lk(connections_mutex);
			connections.push(connection);
			lk.unlock();

			processing_unit_cv.notify_all();
		}

		void SocketServer::Start()
		{
			thread t = thread([&] {
				Run();
			});
			t.detach();
		}

		void SocketServer::Stop()
		{
			if (!processing)
				return;

			processing = false;
			processing_unit_cv.notify_all();

			for (auto t : threads) {
				t->Stop();
			}

			/*
			bool all_threads_stopped = false;
			while (!all_threads_stopped) {
				all_threads_stopped = true;
				for (auto t : threads) {
					all_threads_stopped &= t->IsStopped();
				}
				if (!all_threads_stopped)
					this_thread::sleep_for(chrono::milliseconds(100));
			}
			*/

			while (!stopped)
				this_thread::sleep_for(chrono::milliseconds(100));
		}

		void SocketServer::Dispose()
		{
			Stop();
		}

		void SocketServer::Run()
		{
			processing = true;
			unique_lock<mutex> lk(connections_mutex, defer_lock);
			while (processing) {
				lk.lock();
				processing_unit_cv.wait(lk);
				if (!processing) {
					lk.unlock();
					break;
				}
				
				ostringstream oss;
				oss << "dequeuing connections " << connections.size();
				logger->Debug(__func__, oss.str());
				while (!connections.empty()) {
					shared_ptr<SocketServerThread> available_thread;
					for (auto t : threads) {
						if (!t->IsExecuting()) {
							available_thread = t;
							break;
						}
					}
					if (!available_thread && max_worker_count > threads.size()) {
						logger->Debug(__func__, "creating new server thread");
						available_thread = shared_ptr<SocketServerThread>(new SocketServerThread(processing_unit_cv));
						available_thread->Start();
						threads.push_back(available_thread);
					}
					if (available_thread) {
						logger->Debug(__func__, "assigning connection to thread");
						auto worker = creator->Create();
						auto conn = connections.front();
						connections.pop();
						worker->HandleClient(conn);
						available_thread->Execute(worker);
					} else {
						logger->Debug(__func__, "no thread found for connection");
						break;
					}
				}
				lk.unlock();
			}

			logger->Debug(__func__, "Stopping SocketServer");
			stopped = true;
		}

		void SocketServer::SetWorkerCreator(shared_ptr<SocketServerWorkerCreator> creator)
		{
			this->creator = creator;
		}
	}
}
