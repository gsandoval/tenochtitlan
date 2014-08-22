#include "socket/servertcpsocket.h"
#include "socket/socketexception.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tenochtitlan
{
	using namespace std;

	ServerTcpSocket::ServerTcpSocket() : listening(false), stopped(false)
	{

	}

	ServerTcpSocket::~ServerTcpSocket()
	{
		StopListening();
	}

	void ServerTcpSocket::SetConnectionHandler(
		shared_ptr<TcpClientConnectionHandler> connection_handler)
	{
		this->connection_handler = connection_handler;
	}

	void ServerTcpSocket::Listen(string address, int port)
	{
		struct sockaddr_in serveraddr;
		 
		if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw SocketException("Could not create socket");
		}
		
		int yes = 1;
		if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			throw SocketException("Could not set socket options");
		}
		
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = INADDR_ANY;
		serveraddr.sin_port = htons(port);
		memset(&(serveraddr.sin_zero), '\0', 8);
		 
		if (bind(master_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
		    throw SocketException("Could not bind address to socket");
		}
		 
		if (listen(master_socket, 10) == -1) {
		    throw SocketException("Could not listen on socket");
		}

		thread t = thread([&] {
			StartListening();
		});
		t.detach();
	}

	void ServerTcpSocket::StartListening()
	{
		listening = true;
		fd_set read_fds;
		int fd_max;
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000; // 10 milliseconds

		int fd;
		while (listening) {
			FD_ZERO(&read_fds);
			FD_SET(master_socket, &read_fds);
			fd_max = master_socket;

			for (unsigned int i = 0; i < clients.size(); i++) {
				fd = clients[i]->FileDescriptor();
				if (clients[i]->IsClosed()) {
					// TODO remove from clients collection
				} else if (fd > 0 && !clients[i]->IsSignaled()) {
					FD_SET(fd, &read_fds);
					if (fd > fd_max)
						fd_max = fd;
				}
			}
			
			if (select(fd_max + 1, &read_fds, NULL, NULL, &timeout) == -1) {
				cout << "Exception" << endl;
			    throw SocketException("Error on select");
			}

			if (FD_ISSET(master_socket, &read_fds)) {
				cout << "new connection" << endl;
				auto client = make_shared<TcpClientConnection>();
				clients.push_back(client);
				client->Open(master_socket);

				if (connection_handler) {
					connection_handler->HandleNewConnection(client);
				}
			}

			for (unsigned int i = 0; i < clients.size(); i++) {
				if (FD_ISSET(clients[i]->FileDescriptor(), &read_fds)) {
					cout << "something" << endl;
					clients[i]->SignalEvent();
				}
			}
		}
		cout << "Stopping ServerTcpSocket" << endl;
		stopped = true;
	}

	void ServerTcpSocket::StopListening()
	{
		if (!listening)
			return;
		
		listening = false;
		while (!stopped)
			this_thread::sleep_for(chrono::milliseconds(100));
	}

	void ServerTcpSocket::Dispose()
	{
		StopListening();
	}
}
