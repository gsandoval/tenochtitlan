#ifndef _HTTP_SOCKET_SERVER_WORKER_H_
#define _HTTP_SOCKET_SERVER_WORKER_H_

#include "server/socketserverworker.h"
#include "http/httprequestprocessor.h"

namespace tenochtitlan
{
	namespace server
	{
		class HttpSocketServerWorker : public SocketServerWorker
		{
		private:
			std::shared_ptr<http::HttpRequestProcessor> request_processor;
		public:
			void Execute();
			void ReadReady();
			void WriteReady();

			void SetRequestProcessor(std::shared_ptr<http::HttpRequestProcessor> processor);
		};
	}
}

#endif
