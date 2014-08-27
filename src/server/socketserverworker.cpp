#include "server/socketserverworker.h"

namespace tenochtitlan
{
	using namespace std;

	void SocketServerWorker::HandleClient(shared_ptr<TcpClientConnection> client)
	{
		this->client = client;
	}
}