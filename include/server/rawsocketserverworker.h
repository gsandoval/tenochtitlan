#ifndef _RAW_SOCKET_SERVER_WORKER_H_
#define _RAW_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"

namespace tenochtitlan
{
	class RawSocketServerWorker : public SocketServerWorker
	{
	public:
		void ProcessRequest(std::shared_ptr<TcpClientConnection> client);
	};
}

#endif
