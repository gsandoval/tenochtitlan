#include "server/httpsocketserverworker.h"
#include "parser/httpparser.h"
#include <string>
#include <queue>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		HttpSocketServerWorker::HttpSocketServerWorker()
		{
			logger = shared_ptr<management::Logger>(new management::Logger("HttpSocketServerWorker"));
		}

		void HttpSocketServerWorker::Execute()
		{
			logger->Debug(__func__, "HttpSocketServerWorker::Execute");

			string req_str = "";
			queue<shared_ptr<socket::Buffer>> buffers_read = client->ReadOrWait(10000);
			while (!buffers_read.empty()) {
				req_str.append(buffers_read.front()->Buf(), buffers_read.front()->Size());
				buffers_read.pop();
			}
			logger->Debug(__func__, req_str);
			auto parser = make_shared<parser::HttpParser>();
			shared_ptr<http::HttpEntity> http_request = parser->Parse(req_str);
			shared_ptr<http::HttpEntity> http_response = request_processor->ProcessRequest(http_request);

			client->Close();
		}

		void HttpSocketServerWorker::SetRequestProcessor(
			shared_ptr<http::HttpRequestProcessor> processor)
		{
			request_processor = processor;
		}
	}
}
