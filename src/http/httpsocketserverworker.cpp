#include "http/httpsocketserverworker.h"
#include "parser/httpparser.h"
#include <string>
#include <queue>

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		HttpSocketServerWorker::HttpSocketServerWorker()
		{
			logger = shared_ptr<management::Logger>(new management::Logger("HttpSocketServerWorker"));
		}

		void HttpSocketServerWorker::Execute()
		{
			string req_str = "";
			queue<shared_ptr<socket::Buffer>> buffers_read = client->ReadOrWait(10000);
			while (!buffers_read.empty()) {
				req_str.append(buffers_read.front()->Buf(), buffers_read.front()->Size());
				buffers_read.pop();
			}

			auto parser = make_shared<parser::HttpParser>();
			shared_ptr<http::HttpEntity> http_request = parser->Parse(req_str);

			auto context = make_shared<component::HttpContext>();
			context->SetRequest(http_request);
			context->SetResponse(make_shared<HttpEntity>());
			context->SetConnection(client);

			request_processor->ProcessRequest(context);

			client->Close();
		}

		void HttpSocketServerWorker::SetRequestProcessor(
			shared_ptr<http::HttpRequestProcessor> processor)
		{
			request_processor = processor;
		}
	}
}
