#include "socket/servertcpsocket.h"
#include "socket/socketexception.h"

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
			logger = shared_ptr<management::Logger>(new management::Logger("ServerTcpSocket"));
		}

		ServerTcpSocket::~ServerTcpSocket()
		{
			Dispose();
			logger->Debug(__func__, "~ServerTcpSocket");
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

			int yes = 1;

			if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			    throw SocketException("Error configuring the master socket to be reused");
			}
			
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = INADDR_ANY;
			serveraddr.sin_port = htons(port);
			memset(&(serveraddr.sin_zero), '\0', 8);
			
			fcntl(master_socket, F_SETFL, fcntl(master_socket, F_GETFL, 0) | O_NONBLOCK);

			if (::bind(master_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
				logger->Debug(__func__, "Error binding socket");
			    throw SocketException("Could not bind address to socket");
			}
			 
			if (listen(master_socket, 10) == -1) {
				logger->Debug(__func__, "Error listening to socket");
			    throw SocketException("Could not listen on socket");
			}

			thread t = thread([&] {
				io.set<ServerTcpSocket, &ServerTcpSocket::Accept>(this);
	            io.start(master_socket, ev::READ);

				loop.run(0);
				logger->Debug(__func__, "WHY!");
			});
			t.detach();
		}

		void ServerTcpSocket::Accept(ev::io &watcher, int revents) {
			if (EV_ERROR & revents) {
				logger->Debug(__func__, "An error occurred accepting a connection");
				return;
			}
			io.set(ev::READ);

			auto client = make_shared<TcpClientConnection>();
			client->Open(watcher.fd); // master_socket

			if (connection_handler) {
				connection_handler->HandleNewConnection(client);
			}
        }

		void ServerTcpSocket::Dispose()
		{
			shutdown(master_socket, SHUT_RDWR);
			close(master_socket);
			loop.break_loop();
			logger->Debug(__func__, "Closing master_socket");
		}
	}
}
