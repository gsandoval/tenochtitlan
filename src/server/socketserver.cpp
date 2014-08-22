#include "server/socketserver.h"

#include <iostream>
#include <thread>
#include <chrono>

namespace tenochtitlan
{
	using namespace std;

	SocketServer::SocketServer() : processing(false), stopped(false)
	{

	}

	SocketServer::~SocketServer()
	{
		StopProcessing();
	}

	void SocketServer::HandleNewConnection(shared_ptr<TcpClientConnection> connection)
	{
		connections.push_back(connection);
		cout << "Handling connection" << endl;
	}

	void SocketServer::Start()
	{
		thread t = thread([&] {
			StartProcessing();
		});
		t.detach();
	}

	void SocketServer::Stop()
	{
		StopProcessing();
	}

	void SocketServer::Dispose()
	{
		StopProcessing();
	}

	void SocketServer::StartProcessing()
	{
		processing = true;
		while (processing) {

		}

		cout << "Stopping SocketServer" << endl;
		stopped = true;
	}

	void SocketServer::StopProcessing()
	{
		if (!processing)
			return;

		processing = false;
		while (!stopped)
			this_thread::sleep_for(chrono::milliseconds(100));
	}

	void SocketServer::SetWorkerCreator(shared_ptr<SocketServerWorkerCreator> creator)
	{
		this->creator = creator;
	}
}
