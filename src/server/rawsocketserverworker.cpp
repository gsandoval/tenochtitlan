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
			
		}
	}
}
