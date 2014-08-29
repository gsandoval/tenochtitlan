#ifndef _SOCKET_EXCEPTION_H_
#define _SOCKET_EXCEPTION_H_

#include <exception>
#include <string>

namespace tenochtitlan
{
	namespace socket
	{
		class SocketException : public std::exception
		{
		private:
			std::string message;
			int error_number;
		public:
			SocketException();
			SocketException(std::string msg);
			SocketException(std::string msg, int error_number);

			virtual const char* what() const throw();
		};
	}
}

#endif
