#include "server/rawsocketserverworker.h"

namespace tenochtitlan
{
	using namespace std;

	void RawSocketServerWorker::Execute()
	{
		char buffer[1024];
		int bytes_read = client->Read(buffer, 1024);
		client->Write(buffer, bytes_read);
		client->Close();
	}
}
