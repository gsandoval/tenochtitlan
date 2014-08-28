#include "socket/tcpsocket.h"
#include <iostream>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		TcpSocket::~TcpSocket()
		{
			cout << "~TcpSocket" << endl;
		}
	}
}
