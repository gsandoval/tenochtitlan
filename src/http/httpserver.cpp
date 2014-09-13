#include "http/httpserver.h"
#include "http/httprequestprocessor.h"
#include "http/httpsocketserverworkercreator.h"
#include "http/component/staticresourcecomponent.h"
#include "http/component/restcomponent.h"
#include "http/component/inputvalidationcomponent.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		void HttpServer::Listen(string address, int port)
		{
			auto static_resource_component = make_shared<component::StaticResourceComponent>();
			auto rest_component = make_shared<component::RestComponent>();
			auto input_validation_component = make_shared<component::InputValidationComponent>();

			auto request_processor = make_shared<HttpRequestProcessor>();
			request_processor->AddComponent(input_validation_component);
			request_processor->AddComponent(static_resource_component);
			request_processor->AddComponent(rest_component);

			auto worker_creator = shared_ptr<HttpSocketServerWorkerCreator>(new HttpSocketServerWorkerCreator(request_processor));

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

