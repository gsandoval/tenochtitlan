#include "server/httpsocketserverworker.h"
#include <string>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		void HttpSocketServerWorker::Execute()
		{
			char buffer[256];
			auto http_entity = make_shared<HttpEntity>();
			
			string req_str = "";
			bool receiving_request = true;
			while (receiving_request) {
				int bytes_read = client->Read(buffer, 255, 100);
				if (bytes_read > 0) {
					req_str.append(buffer, bytes_read);
				}
				if ((req_str != "" && bytes_read == 0) || client->IsClosed()) {
					receiving_request = false;
				}
			}
			HttpParser
		}

		void HttpSocketServerWorker::SetRequestProcessor(
			shared_ptr<http::HttpRequestProcessor> processor)
		{
			request_processor = processor;
		}
	}
}
