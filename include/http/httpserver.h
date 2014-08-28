#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "management/disposable.h"
#include "socket/servertcpsocket.h"
#include "server/socketserver.h"
#include <string>

namespace tenochtitlan
{
	namespace http
	{
		class HttpServer : public management::Disposable
		{
		private:
			std::shared_ptr<socket::ServerTcpSocket> master_socket;
			std::shared_ptr<server::SocketServer> server;
		public:
			void Listen(std::string address, int port);
			void Dispose();
		};
	}
}

#endif
