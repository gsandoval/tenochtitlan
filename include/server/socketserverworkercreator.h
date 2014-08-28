#ifndef _SOCKET_SERVER_WORKER_CREATOR_H_
#define _SOCKET_SERVER_WORKER_CREATOR_H_

#include "socketserverworker.h"
#include <memory>

namespace tenochtitlan
{
	namespace server
	{
		class SocketServerWorkerCreator
		{
		public:
			virtual std::shared_ptr<SocketServerWorker> Create() = 0;
		};
	}
}

#endif
