#ifndef _HTTP_SOCKET_SERVER_WORKER_CREATOR_H_
#define _HTTP_SOCKET_SERVER_WORKER_CREATOR_H_

#include "server/socketserverworkercreator.h"
#include "httpsocketserverworker.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		class HttpSocketServerWorkerCreator : public server::SocketServerWorkerCreator
		{
		private:
			std::shared_ptr<HttpRequestProcessor> processor;
		public:
			HttpSocketServerWorkerCreator(std::shared_ptr<HttpRequestProcessor> processor);

			std::shared_ptr<server::SocketServerWorker> Create();
		};
	}
}

#endif
