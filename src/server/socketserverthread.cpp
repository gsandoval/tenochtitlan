#include "server/socketserverthread.h"

#include <thread>

namespace tenochtitlan
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
	}
		
	void SocketServerThread::Execute(shared_ptr<SocketServerWorker> worker)
	{
		current_worker = worker;
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
		unique_lock<mutex> lk(idle_mutex);
		while (running) {
			idle_thread.wait(lk);
			if (!running)
				break;

			executing = true;
			current_worker->Execute();
			executing = false;
			
			processing_unit_cv.notify_all();
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
