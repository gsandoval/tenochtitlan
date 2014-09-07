#ifndef _HTTP_SOCKET_SERVER_WORKER_H_
#define _HTTP_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"
#include "http/httprequestprocessor.h"
#include "management/logger.h"

namespace tenochtitlan
{
	namespace server
	{
		class HttpSocketServerWorker : public SocketServerWorker
		{
		private:
			std::shared_ptr<http::HttpRequestProcessor> request_processor;
			std::shared_ptr<management::Logger> logger;
		public:
			HttpSocketServerWorker();

			void Execute();
			void ReadReady();
			void WriteReady();

			void SetRequestProcessor(std::shared_ptr<http::HttpRequestProcessor> processor);
		};
	}
}

#endif
