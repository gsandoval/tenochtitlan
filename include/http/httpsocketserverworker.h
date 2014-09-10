#ifndef _HTTP_SOCKET_SERVER_WORKER_H_
#define _HTTP_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"
#include "httprequestprocessor.h"
#include "management/logger.h"

namespace tenochtitlan
{
	namespace http
	{
		class HttpSocketServerWorker : public server::SocketServerWorker
		{
		private:
			std::shared_ptr<HttpRequestProcessor> request_processor;
			std::shared_ptr<management::Logger> logger;
		public:
			HttpSocketServerWorker();

			void Execute();

			void SetRequestProcessor(std::shared_ptr<HttpRequestProcessor> processor);
		};
	}
}

#endif
