#include "server/rawsocketserverworker.h"
#include <iostream>

namespace tenochtitlan
{
	namespace server
	{
		using namespace std;

		RawSocketServerWorker::~RawSocketServerWorker()
		{
			cout << "~RawSocketServerWorker" << endl;
		}

		void RawSocketServerWorker::Execute()
		{
			char buffer[1024];
			int bytes_read = client->Read(buffer, 1024);
			cout << "buffer content " << buffer << endl << flush;
			client->Write(buffer, bytes_read);
			client->Close();
		}
	}
}
