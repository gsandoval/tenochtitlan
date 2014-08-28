#ifndef _SERVER_TCP_SOCKET_H_
#define _SERVER_TCP_SOCKET_H_

#include "tcpsocket.h"
#include "tcpclientconnection.h"
#include "tcpclientconnectionhandler.h"
#include "management/disposable.h"

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
			bool listening;
			bool stopped;
			int master_socket;
			std::vector<std::shared_ptr<TcpClientConnection>> clients;
			std::shared_ptr<TcpClientConnectionHandler> connection_handler;

			void Run();
			void Stop();
		public:
			ServerTcpSocket();
			~ServerTcpSocket();

			void SetConnectionHandler(std::shared_ptr<TcpClientConnectionHandler> connection_handler);
			void Listen(std::string address, int port);
			void Dispose();
		};
	}
}

#endif
