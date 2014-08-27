#ifndef _SOCKET_SERVER_WORKER_H_
#define _SOCKET_SERVER_WORKER_H_

#include "socket/tcpclientconnection.h"
#include <memory>

namespace tenochtitlan
{
	class SocketServerWorker
	{
	protected:
		std::shared_ptr<TcpClientConnection> client;
	public:
		void HandleClient(std::shared_ptr<TcpClientConnection> client);
		virtual void Execute() = 0;
	};
}

#endif
