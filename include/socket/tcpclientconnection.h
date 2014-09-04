
#ifndef _TCP_CLIENT_CONNECTION_H_
#define _TCP_CLIENT_CONNECTION_H_

#include <ev++.h>
#include <queue>

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
			std::queue<Buffer> write_queue;
		public:
			TcpClientConnection();
			~TcpClientConnection();

			int FileDescriptor();
			void Open(int master_socket);
			void Close();
			int Read(char* buf, int buffer_size);
			void Write(char *buf, int buffer_size);
			void DoRead();
			void DoWrite();
			void SignalEvent(ev::io &watcher, int revents);
			bool IsClosed();
		};
	}
}

#endif
