#include "socket/socketexception.h"
#include <sstream>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		SocketException::SocketException() : error_number(0)
		{

		}

		SocketException::SocketException(string message) : message(message), error_number(0)
		{

		}

		SocketException::SocketException(string message, int error_number) : message(message), error_number(error_number)
		{

		}

		const char* SocketException::what() const throw()
		{
			ostringstream oss;
			oss << message;
			if (error_number) {
				oss << " " << error_number;
			}
			return oss.str().c_str();
		}
	}
}
