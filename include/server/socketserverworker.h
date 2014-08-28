#ifndef _SOCKET_SERVER_WORKER_H_
#define _SOCKET_SERVER_WORKER_H_

#include "socket/tcpclientconnection.h"
#include <memory>

namespace tenochtitlan
{
	namespace server
	{
		class SocketServerWorker
		{
		protected:
			std::shared_ptr<socket::TcpClientConnection> client;
		public:
			void HandleClient(std::shared_ptr<socket::TcpClientConnection> client);
			virtual void Execute() = 0;
		};
	}
}

#endif
