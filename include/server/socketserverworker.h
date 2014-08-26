#ifndef _SOCKET_SERVER_WORKER_H_
#define _SOCKET_SERVER_WORKER_H_

#include "socket/tcpclientconnection.h"
#include <memory>

namespace tenochtitlan
{
	class SocketServerWorker
	{
	public:
		virtual void HandleClient(std::shared_ptr<TcpClientConnection> client) = 0;
	};
}

#endif
