#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include "socket/servertcpsocket.h"
#include "management/applicationlifecyclelistener.h"
#include "server/socketserver.h"
#include "server/socketserverworkercreator.h"
#include "server/rawsocketserverworker.h"

using namespace std;
using namespace tenochtitlan;

class RawSocketServerWorkerCreator : public SocketServerWorkerCreator
{
public:
	shared_ptr<SocketServerWorker> Create()
	{
		return make_shared<RawSocketServerWorker>();
	}
};

int main(int argc, char *argv[])
{
	int PORT = 0;
	if (argc > 1) {
		string port_str(argv[1]);
		istringstream iss(port_str);
		iss >> PORT;
	}
	if (PORT == 0) {
		cout << "A port must be specified" << endl;
		return 0;
	}

	auto lifecycle_listener = ApplicationLifecycleListener::Instance();

	auto master_socket = make_shared<ServerTcpSocket>();

	auto server = make_shared<SocketServer>();
	server->SetWorkerCreator(make_shared<RawSocketServerWorkerCreator>());
	server->Start();
	master_socket->SetConnectionHandler(server);

	try {
		master_socket->Listen("localhost", PORT);
	} catch (exception& e) {
		cout << e.what() << endl;
	}

	lifecycle_listener->AddToGlobalPool(master_socket);
	lifecycle_listener->AddToGlobalPool(server);

	cin.ignore(numeric_limits<streamsize>::max());
	cin.get();
	cout << "Exiting" << endl;

	return 0;
}