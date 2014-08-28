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
		public:
			SocketException();
			SocketException(std::string msg);

			virtual const char* what() const throw();
		};
	}
}

#endif
