#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include "socket/tcpclientconnectionhandler.h"

#include "server/socketserverworkercreator.h"
#include "management/disposable.h"
#include <vector>

namespace tenochtitlan
{
	class SocketServer : public TcpClientConnectionHandler, public Disposable
	{
	private:
		bool processing;
		bool stopped;
		std::vector<std::shared_ptr<TcpClientConnection>> connections;
		std::shared_ptr<SocketServerWorkerCreator> creator;

		void StartProcessing();
		void StopProcessing();
	public:
		SocketServer();
		~SocketServer();

		void HandleNewConnection(std::shared_ptr<TcpClientConnection> connection);
		void Start();
		void Stop();
		void Dispose();
		void SetWorkerCreator(std::shared_ptr<SocketServerWorkerCreator> creator);
	};
}

#endif
