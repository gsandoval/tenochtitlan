#include "socket/servertcpsocket.h"
#include "socket/socketexception.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		ServerTcpSocket::ServerTcpSocket() : listening(false), stopped(false)
		{

		}

		ServerTcpSocket::~ServerTcpSocket()
		{
			Dispose();
			cout << "~ServerTcpSocket" << endl;
		}

		void ServerTcpSocket::SetConnectionHandler(
			shared_ptr<TcpClientConnectionHandler> connection_handler)
		{
			this->connection_handler = connection_handler;
		}

		void ServerTcpSocket::Listen(string address, int port)
		{
			struct sockaddr_in serveraddr;
			 
			if ((master_socket = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				throw SocketException("Could not create socket");
			}
			
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = INADDR_ANY;
			serveraddr.sin_port = htons(port);
			memset(&(serveraddr.sin_zero), '\0', 8);
			
			fcntl(master_socket, F_SETFL, fcntl(master_socket, F_GETFL, 0) | O_NONBLOCK);

			if (::bind(master_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
			    throw SocketException("Could not bind address to socket");
			}
			 
			if (listen(master_socket, 10) == -1) {
			    throw SocketException("Could not listen on socket");
			}

			io.set<ServerTcpSocket, &ServerTcpSocket::Accept>(this);
            io.start(master_socket, ev::READ);

            
			thread t = thread([&] {
				loop.run(0);
			});
			t.detach();
		}

		void ServerTcpSocket::Accept(ev::io &watcher, int revents) {
            cout << "new connection" << endl;
			auto client = make_shared<TcpClientConnection>();
			clients.push_back(client);
			client->Open(watcher.fd); // master_socket

			if (connection_handler) {
				connection_handler->HandleNewConnection(client);
			}
        }

		void ServerTcpSocket::Dispose()
		{
			loop.break_loop();
			shutdown(master_socket, SHUT_RDWR);
			close(master_socket);
		}
	}
}
