#include "server/socketserverworker.h"

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		void SocketServerWorker::HandleClient(shared_ptr<socket::TcpClientConnection> client)
		{
			this->client = client;
		}
	}
}