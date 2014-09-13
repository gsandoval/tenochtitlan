#include "http/httpsocketserverworker.h"
#include "parser/httpparser.h"
#include <string>
#include <queue>
#include <sstream>

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
			queue<shared_ptr<socket::Buffer>> buffers_read = client->ReadOrWait(10000);

			auto parser = make_shared<parser::HttpParser>();
			auto http_request = make_shared<http::HttpEntity>();
			auto bytes_left = parser->Parse(buffers_read, http_request);
			if (bytes_left->Size() > 0) {
				client->RequeueBuffer(bytes_left);
			}

			auto context = make_shared<component::HttpContext>();
			context->SetRequest(http_request);
			context->SetResponse(make_shared<HttpEntity>());
			context->SetConnection(client);

			request_processor->ProcessRequest(context);
			auto props = context->Properties();
			if (props->GetBool("t:IsHandled")) {
				auto response = context->Response();
				//response->AddHeader("Transfer-Encoding", "chunked");

				ostringstream oss;
				oss << response->Version() << " " << response->Code() << " " << response->CodeStr() << "\n\r";

				auto header_names = response->HeaderNames();
				for (unsigned int i = 0; i < header_names.size(); i++) {
					oss << header_names[i] << ": " << response->Header(header_names[i]) << "\n\r";
				}
				oss << "\n\r";
				client->Write(oss.str());

				auto content = response->ContentAsBuffer();
				oss = ostringstream();
				oss << "content size " << content->Size();
				logger->Debug(__func__, oss.str());
				client->Write(content);
			}

			client->Close();
		}

		void HttpSocketServerWorker::SetRequestProcessor(
			shared_ptr<http::HttpRequestProcessor> processor)
		{
			request_processor = processor;
		}
	}
}
