#ifndef _RAW_SOCKET_SERVER_WORKER_H_
#define _RAW_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"

namespace tenochtitlan
{
	namespace server
	{
		class RawSocketServerWorker : public SocketServerWorker
		{
		public:
			~RawSocketServerWorker();
			void Execute();
			void ReadReady();
			void WriteReady();
		};
	}
}

#endif
