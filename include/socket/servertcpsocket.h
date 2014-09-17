#ifndef _SERVER_TCP_SOCKET_H_
#define _SERVER_TCP_SOCKET_H_

#include "tcpsocket.h"
#include "tcpclientconnection.h"
#include "tcpclientconnectionhandler.h"
#include "management/disposable.h"
#include "management/logger.h"

#include <uv.h>

#include <string>
#include <vector>
#include <memory>
#include <thread>

namespace tenochtitlan
{
	namespace socket
	{
		class ServerTcpSocket : public TcpSocket, public management::Disposable
		{
		private:
			uv_loop_t loop;
			uv_tcp_t *server;

			bool listening;
			bool stopped;
			int master_socket;
			std::shared_ptr<TcpClientConnectionHandler> connection_handler;
			std::shared_ptr<management::Logger> logger;

			void Run();
			void Stop();
		public:
			ServerTcpSocket();
			~ServerTcpSocket();

			void SetConnectionHandler(std::shared_ptr<TcpClientConnectionHandler> connection_handler);
			void Listen(std::string address, int port);
			void Dispose();

			void Accept(int socket_fd, int revents);

			friend void native_accept(struct ev_loop *loop, ev_io *w, int revents);
		};
	}
}

#endif
