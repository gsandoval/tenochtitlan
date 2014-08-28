#include "socket/socketexception.h"

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		SocketException::SocketException()
		{

		}

		SocketException::SocketException(string message) : message(message)
		{

		}

		const char* SocketException::what() const throw()
		{
			return message.c_str();
		}
	}
}
