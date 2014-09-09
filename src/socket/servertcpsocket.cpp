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
			loop = ev_default_loop(0);
			ev_set_userdata(loop, this);
			io = new ev_io();
		}

		ServerTcpSocket::~ServerTcpSocket()
		{
			delete io;
			Dispose();
			logger->Debug(__func__, "~ServerTcpSocket");
		}

		void ServerTcpSocket::SetConnectionHandler(
			shared_ptr<TcpClientConnectionHandler> connection_handler)
		{
			this->connection_handler = connection_handler;
		}

		void native_accept(struct ev_loop *loop, ev_io *w, int revents)
		{
			void *data = ev_userdata(loop);
			ServerTcpSocket *server = (ServerTcpSocket *)data;
			server->logger->Debug(__func__, "Something");
			server->Accept(w->fd, revents);
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

			logger->Debug(__func__, "Before init");
			ev_io_init(io, native_accept, master_socket, EV_READ);
			logger->Debug(__func__, "Before start");
			ev_io_start(loop, io);
			logger->Debug(__func__, "After start");

			thread t = thread([&] {
				ev_run(loop, 0);
				logger->Debug(__func__, "WHY!");
			});
			t.detach();
		}

		void ServerTcpSocket::Accept(int master_fd, int revents) {
			if (EV_ERROR & revents) {
				logger->Debug(__func__, "An error occurred accepting a connection");
				return;
			}

			auto client = make_shared<TcpClientConnection>();
			client->Open(master_fd);

			if (connection_handler) {
				connection_handler->HandleNewConnection(client);
			}
        }

		void ServerTcpSocket::Dispose()
		{
			ev_io_stop(loop, io);

			shutdown(master_socket, SHUT_RDWR);
			close(master_socket);
			
			ev_break(loop, EVBREAK_ONE);
			logger->Debug(__func__, "Closing master_socket");
		}
	}
}
