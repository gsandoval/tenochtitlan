#ifndef _SOCKET_SERVER_THREAD_H_
#define _SOCKET_SERVER_THREAD_H_

#include "socketserverworker.h"
#include <memory>
#include <condition_variable>

namespace tenochtitlan
{
	namespace server
	{
		class SocketServerThread
		{
		private:
			bool executing;
			bool stopped;
			bool running;
			std::condition_variable processing_unit_cv;
			std::shared_ptr<SocketServerWorker> current_worker;
			std::condition_variable idle_thread;
			std::mutex idle_mutex;
			std::mutex stop_wait_mutex;
			std::condition_variable stop_wait;

			void Run();
		public:
			SocketServerThread();
			SocketServerThread(std::condition_variable& processing_unit_cv);
			~SocketServerThread();
			
			void Execute(std::shared_ptr<SocketServerWorker> worker);
			void Start();
			void Stop();
			bool IsStopped();
			bool IsExecuting();
		};
	}
}

#endif
