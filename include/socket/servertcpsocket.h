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
#include <mutex>

namespace tenochtitlan
{
	namespace socket
	{
		class ServerTcpSocket : public TcpSocket, public management::Disposable
		{
		private:
			uv_tcp_t *server;

			bool listening;
			bool stopped;
			bool disposed;
			int master_socket;
			std::shared_ptr<TcpClientConnectionHandler> connection_handler;
			std::shared_ptr<management::Logger> logger;
			std::mutex disposed_mutex;

			void Run();
			void Stop();
		public:
			ServerTcpSocket();
			~ServerTcpSocket();

			void SetConnectionHandler(std::shared_ptr<TcpClientConnectionHandler> connection_handler);
			void Listen(std::string address, int port);
			void Dispose();

			void Accept(uv_stream_t *server, int status);

			friend void native_accept(uv_stream_t *server, int status);
		};
	}
}

#endif
