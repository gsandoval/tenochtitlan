#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include "socket/tcpclientconnectionhandler.h"
#include "server/socketserverthread.h"
#include "server/socketserverworkercreator.h"
#include "management/disposable.h"
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>

namespace tenochtitlan
{
	namespace server
	{
		class SocketServer : public socket::TcpClientConnectionHandler, public management::Disposable
		{
		private:
			bool processing;
			bool stopped;
			unsigned int max_worker_count;
			std::queue<std::shared_ptr<socket::TcpClientConnection>> connections;
			std::shared_ptr<SocketServerWorkerCreator> creator;
			std::vector<std::shared_ptr<SocketServerThread>> threads;
			std::condition_variable processing_unit_cv;
			std::mutex connections_mutex;

			void Run();
		public:
			SocketServer();
			~SocketServer();

			void HandleNewConnection(std::shared_ptr<socket::TcpClientConnection> connection);
			void Start();
			void Stop();
			void Dispose();
			void SetWorkerCreator(std::shared_ptr<SocketServerWorkerCreator> creator);
		};
	}
}

#endif
