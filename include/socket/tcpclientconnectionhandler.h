#ifndef _TCP_CLIENT_CONNECTION_HANDLER_H_
#define _TCP_CLIENT_CONNECTION_HANDLER_H_

#include "socket/tcpclientconnection.h"

#include <memory>

namespace tenochtitlan
{
	namespace socket
	{
		class TcpClientConnectionHandler
		{
		public:
			virtual void HandleNewConnection(std::shared_ptr<TcpClientConnection> connection) = 0;
		};
	}
}

#endif

