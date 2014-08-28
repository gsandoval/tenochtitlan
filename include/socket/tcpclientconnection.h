
#ifndef _TCP_CLIENT_CONNECTION_H_
#define _TCP_CLIENT_CONNECTION_H_

namespace tenochtitlan
{
	namespace socket
	{
		class TcpClientConnection
		{
		private:
			bool signaled;
			bool closed;
			int socket_fd;
		public:
			TcpClientConnection();
			~TcpClientConnection();

			int FileDescriptor();
			void Open(int master_socket);
			void Close();
			int Read(char* buf, int buffer_size, int timeout = 0);
			void Write(char *buf, int buffer_size);
			bool IsSignaled();
			void SignalEvent();
			bool IsClosed();
		};
	}
}

#endif
