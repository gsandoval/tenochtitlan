#ifdef _MSC_VER
#define NOMINMAX
#endif
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include "http/httpserver.h"
#include "management/applicationlifecyclelistener.h"
#include "management/logger.h"

using namespace std;
using namespace tenochtitlan::http;
using namespace tenochtitlan::management;

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

	auto http_server = make_shared<HttpServer>();

	auto lifecycle_listener = ApplicationLifecycleListener::Instance();
	lifecycle_listener->AddToGlobalPool(http_server);
	
	try {
		http_server->Listen("localhost", PORT);
	} catch (exception& e) {
		cout << e.what() << endl;
	}

	auto logger = shared_ptr<Logger>(new Logger("Server"));
	ostringstream oss;
	oss << "Executing http server on port " << PORT;
	logger->Info(__func__, oss.str());

	cin.ignore(numeric_limits<streamsize>::max());
	cin.get();
	cout << "Exiting" << endl;

	return 0;
}