#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include "socket/servertcpsocket.h"
#include "management/applicationlifecyclelistener.h"

using namespace std;
using namespace tenochtitlan;

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

	auto server_socket = make_shared<ServerTcpSocket>();
	try {
		server_socket->Listen("localhost", PORT);
	} catch (exception& e) {
		cout << e.what() << endl;
	}

	lifecycle_listener->AddToGlobalPool(server_socket);

	cin.ignore(numeric_limits<streamsize>::max());
	cin.get();
	cout << "Exiting" << endl;

	return 0;
}