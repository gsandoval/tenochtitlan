#include "server/socketserverthread.h"
#include <iostream>
#include <thread>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		SocketServerThread::SocketServerThread(condition_variable& processing_unit_cv) : 
			executing(false), stopped(false), running(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("SocketServerThread"));
		}
		
		SocketServerThread::~SocketServerThread()
		{
			Stop();
		}
			
		void SocketServerThread::Execute(shared_ptr<SocketServerWorker> worker)
		{
			executing = true;

			unique_lock<mutex> lk(idle_mutex);
			current_worker = worker;
			lk.unlock();

			idle_thread.notify_all();
		}

		void SocketServerThread::Start()
		{
			thread t = thread([&] {
				Run();
			});
			t.detach();
		}

		void SocketServerThread::Stop()
		{
			if (!running)
				return;
			
			running = false;
			idle_thread.notify_all();

			while (!stopped)
				this_thread::sleep_for(chrono::milliseconds(100));
		}

		void SocketServerThread::Run()
		{
			running = true;
			unique_lock<mutex> lk(idle_mutex, defer_lock);
			while (running) {
				lk.lock();
				if (!current_worker) {
					idle_thread.wait(lk);
				}
				if (!running) {
					lk.unlock();
					break;
				}

				current_worker->Execute();
				
				processing_unit_cv.notify_all();

				current_worker = nullptr;
				lk.unlock();
				executing = false;
			}

			stopped = true;
		}

		bool SocketServerThread::IsStopped()
		{
			return stopped;
		}

		bool SocketServerThread::IsExecuting()
		{
			return executing;
		}
	}
}
