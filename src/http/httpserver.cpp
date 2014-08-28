#include "http/httpserver.h"
#include "http/httprequestprocessor.h"
#include "http/httpsocketserverworkercreator.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		void HttpServer::Listen(string address, int port)
		{
			auto request_processor = make_shared<HttpRequestProcessor>();

			auto worker_creator = shared_ptr<HttpSocketServerWorkerCreator>(
				new HttpSocketServerWorkerCreator(request_processor));

			server = make_shared<server::SocketServer>();
			server->SetWorkerCreator(worker_creator);
			server->Start();

			master_socket = make_shared<socket::ServerTcpSocket>();
			master_socket->SetConnectionHandler(server);
			master_socket->Listen(address, port);
		}

		void HttpServer::Dispose()
		{
			server->Dispose();
			master_socket->Dispose();
		}
	}
}

