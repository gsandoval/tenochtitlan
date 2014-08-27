#ifndef _HTTP_SOCKET_SERVER_WORKER_H_
#define _HTTP_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"

namespace tenochtitlan
{
	class HttpSocketServerWorker : public SocketServerWorker
	{
	public:
		void Execute();
	};
}

#endif
