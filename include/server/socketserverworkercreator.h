#ifndef _SOCKET_SERVER_WORKER_CREATOR_H_
#define _SOCKET_SERVER_WORKER_CREATOR_H_

#include "socketserverworker.h"
#include <memory>

namespace tenochtitlan
{
	class SocketServerWorkerCreator
	{
		virtual std::shared_ptr<SocketServerWorker> Create() = 0;
	};
}

#endif
