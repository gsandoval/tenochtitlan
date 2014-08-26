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
		while (!stopped)
			this_thread::sleep_for(chrono::milliseconds(100));
	}

	void SocketServerThread::Run()
	{
		running = true;
		while (running) {

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
