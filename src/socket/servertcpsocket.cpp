#include "socket/servertcpsocket.h"
#include "socket/socketexception.h"

#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		ServerTcpSocket::ServerTcpSocket() : listening(false), stopped(false), disposed(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("ServerTcpSocket"));
			server = new uv_tcp_t();
			uv_tcp_init(uv_default_loop(), server);
			server->data = this;
		}

		ServerTcpSocket::~ServerTcpSocket()
		{
			Dispose();

			delete server;
		}

		void ServerTcpSocket::SetConnectionHandler(shared_ptr<TcpClientConnectionHandler> connection_handler)
		{
			this->connection_handler = connection_handler;
		}

		void native_accept(uv_stream_t *server, int status)
		{
			ServerTcpSocket *that = (ServerTcpSocket*)server->data;
			that->Accept(server, status);
		}

		void ServerTcpSocket::Listen(string address, int port)
		{
			struct sockaddr_in serveraddr;
			uv_ip4_addr("0.0.0.0", port, &serveraddr);
		    uv_tcp_bind(server, (const struct sockaddr *)&serveraddr, 0);

		    int r = uv_listen((uv_stream_t*) server, 128, native_accept);
		    if (r) {
		    	ostringstream oss;
		    	oss << "Error listening on socket " << uv_err_name(errno);
		        throw SocketException(oss.str());
		    }

			thread t = thread([&] {
				uv_run(uv_default_loop(), UV_RUN_DEFAULT);
			});
			t.detach();
		}

		void ServerTcpSocket::Accept(uv_stream_t *server, int status) {
			if (status == -1) {
		        throw SocketException("Error reading from socket");
		    }

			auto client = make_shared<TcpClientConnection>();
			client->Open(server);

			if (connection_handler) {
				connection_handler->HandleNewConnection(client);
			} else {
				logger->Warn(__func__, "A connection was received but no handler was registered");
			}
        }

		void ServerTcpSocket::Dispose()
		{
			unique_lock<mutex> lk(disposed_mutex);
			if (!disposed) {
				disposed = true;
				uv_close((uv_handle_t*)server, NULL);
			}
			lk.unlock();
		}
	}
}
