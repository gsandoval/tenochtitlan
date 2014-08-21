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
	class ServerTcpSocket : public TcpSocket, public Disposable
	{
	private:
		bool listening;
		bool stopped;
		int master_socket;
		std::shared_ptr<std::thread> listening_thread;
		std::vector<std::shared_ptr<TcpClientConnection>> clients;
		std::shared_ptr<TcpClientConnectionHandler> connection_handler;

		void StartListening();
		void StopListening();
	public:
		ServerTcpSocket();
		~ServerTcpSocket();

		void SetConnectionHandler(std::shared_ptr<TcpClientConnectionHandler> connection_handler);
		void Listen(std::string address, int port);
		void Dispose();
	};
}

#endif
