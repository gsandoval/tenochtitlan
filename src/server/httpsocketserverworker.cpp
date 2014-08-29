#include "server/httpsocketserverworker.h"
#include "parser/httpparser.h"
#include <string>
#include <iostream>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		void HttpSocketServerWorker::Execute()
		{
			int buflen = 255;
			char buffer[256];
			
			string req_str = "";
			bool receiving_request = true;
			cout << "HttpSocketServerWorker::Execute" << endl;
			while (receiving_request) {
				int bytes_read = client->Read(buffer, buflen, 100);
				if (bytes_read > 0) {
					req_str.append(buffer, bytes_read);
				}
				if ((req_str != "" && bytes_read == 0) || client->IsClosed()) {
					receiving_request = false;
				}
			}
			cout << "End loop" << endl;
			cout << req_str << endl;
			auto parser = make_shared<parser::HttpParser>();
			shared_ptr<http::HttpEntity> http_request = parser->Parse(req_str);
			shared_ptr<http::HttpEntity> http_response = request_processor->ProcessRequest(http_request);

			//client->Write();
		}

		void HttpSocketServerWorker::SetRequestProcessor(
			shared_ptr<http::HttpRequestProcessor> processor)
		{
			request_processor = processor;
		}
	}
}
