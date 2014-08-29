
#ifndef _TCP_CLIENT_CONNECTION_H_
#define _TCP_CLIENT_CONNECTION_H_

#include <ev++.h>

namespace tenochtitlan
{
	namespace socket
	{
		class TcpClientConnection
		{
		private:
			bool closed;
			int socket_fd;
			ev::io io;
		public:
			TcpClientConnection();
			~TcpClientConnection();

			int FileDescriptor();
			void Open(int master_socket);
			void Close();
			int Read(char* buf, int buffer_size, int timeout = 0);
			void Write(char *buf, int buffer_size);
			void SignalEvent(ev::io &watcher, int revents);
			bool IsClosed();
		};
	}
}

#endif
