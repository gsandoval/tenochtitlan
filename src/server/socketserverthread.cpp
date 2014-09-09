#include "server/socketserverthread.h"
#include <iostream>
#include <thread>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		SocketServerThread::SocketServerThread() : 
			executing(false), stopped(false), running(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("SocketServerThread"));
		}

		SocketServerThread::SocketServerThread(condition_variable& processing_unit_cv) : 
			executing(false), stopped(false), running(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("SocketServerThread"));
		}
		
		SocketServerThread::~SocketServerThread()
		{
			Stop();
			logger->Debug(__func__, "~SocketServerThread");
		}
			
		void SocketServerThread::Execute(shared_ptr<SocketServerWorker> worker)
		{
			executing = true;

			unique_lock<mutex> lk(idle_mutex);
			current_worker = worker;
			lk.unlock();

			idle_thread.notify_all();
			logger->Debug(__func__, "notify idle thread");
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
			logger->Debug(__func__, "before stop notify all");
			idle_thread.notify_all();
			logger->Debug(__func__, "after stop notify all");

			/*
			unique_lock<mutex> stop_lock(stop_wait_mutex);
			if (!stopped)
				stop_wait.wait(stop_lock);
			stop_lock.unlock();
			*/

			while (!stopped)
				this_thread::sleep_for(chrono::milliseconds(100));
			logger->Debug(__func__, "Exiting stop method");
		}

		void SocketServerThread::Run()
		{
			running = true;
			unique_lock<mutex> lk(idle_mutex, defer_lock);
			while (running) {
				lk.lock();
				if (!current_worker) {
					logger->Debug(__func__, "before idle_thread");
					idle_thread.wait(lk);
					logger->Debug(__func__, "idle_thread woke up");
				}
				if (!running) {
					lk.unlock();
					logger->Debug(__func__, "idle_thread unlocked");
					break;
				}

				logger->Debug(__func__, "before execute");
				current_worker->Execute();
				logger->Debug(__func__, "after execute");
				
				processing_unit_cv.notify_all();
				logger->Debug(__func__, "after notifying execute finished");

				current_worker = nullptr;
				lk.unlock();
				logger->Debug(__func__, "idle_thread unlocked");
				executing = false;
			}

			logger->Debug(__func__, "Stopping SocketServerThread");

			/*
			unique_lock<mutex> stop_lock(stop_wait_mutex);
			*/
			stopped = true;
			/*
			stop_lock.unlock();
			stop_wait.notify_all();
			*/
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
