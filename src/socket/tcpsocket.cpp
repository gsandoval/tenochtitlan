#include "socket/tcpsocket.h"
#include <iostream>

namespace tenochtitlan
{
	using namespace std;

	TcpSocket::~TcpSocket()
	{
		cout << "~TcpSocket" << endl;
	}
}
