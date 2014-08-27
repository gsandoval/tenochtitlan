#ifndef _RAW_SOCKET_SERVER_WORKER_H_
#define _RAW_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"

namespace tenochtitlan
{
	class RawSocketServerWorker : public SocketServerWorker
	{
	public:
		void Execute();
	};
}

#endif
