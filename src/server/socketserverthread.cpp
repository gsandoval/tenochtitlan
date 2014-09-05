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

		}

		SocketServerThread::SocketServerThread(condition_variable& processing_unit_cv) : 
			executing(false), stopped(false), running(false)
		{

		}
		
		SocketServerThread::~SocketServerThread()
		{
			Stop();
			cout << "~SocketServerThread" << endl;
		}
			
		void SocketServerThread::Execute(shared_ptr<SocketServerWorker> worker)
		{
			executing = true;

			unique_lock<mutex> lk(idle_mutex);
			current_worker = worker;
			lk.unlock();

			idle_thread.notify_all();
			cout << "notify idle thread" << endl;
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
			cout << "before stop notify all" << endl;
			idle_thread.notify_all();
			cout << "after stop notify all" << endl;

			unique_lock<mutex> stop_lock(stop_wait_mutex);
			if (!stopped)
				stop_wait.wait(stop_lock);
			stop_lock.unlock();

			/*
			while (!stopped)
				this_thread::sleep_for(chrono::milliseconds(100));
			*/
			cout << "Exiting stop method" << endl;
		}

		void SocketServerThread::Run()
		{
			running = true;
			unique_lock<mutex> lk(idle_mutex, defer_lock);
			while (running) {
				lk.lock();
				if (!current_worker) {
					cout << "before idle_thread" << endl;
					idle_thread.wait(lk);
					cout << "idle_thread woke up" << endl;
				}
				if (!running) {
					lk.unlock();
					cout << "idle_thread unlocked" << endl;
					break;
				}

				cout << "before execute" << endl;
				current_worker->Execute();
				cout << "after execute" << endl;
				
				processing_unit_cv.notify_all();
				cout << "after notifying execute finished" << endl;

				current_worker = nullptr;
				lk.unlock();
				cout << "idle_thread unlocked" << endl;
				executing = false;
			}

			cout << "Stopping SocketServerThread" << endl;

			unique_lock<mutex> stop_lock(stop_wait_mutex);
			stopped = true;
			stop_lock.unlock();
			stop_wait.notify_all();
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
