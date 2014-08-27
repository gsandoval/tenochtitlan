#include "socket/tcpclientconnection.h"
#include "socket/socketexception.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tenochtitlan
{

	TcpClientConnection::TcpClientConnection() : signaled(false), closed(false)
	{
	}

	TcpClientConnection::~TcpClientConnection()
	{

	}

	void TcpClientConnection::Open(int master_socket) {
		struct sockaddr_in clientaddr;
		socklen_t addrlen = sizeof(clientaddr);
		socket_fd = ::accept(master_socket, (struct sockaddr *)&clientaddr, &addrlen);
		if (socket_fd == -1)
			throw SocketException("Could not accept connection");
		//printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), socket_fd);
	}

	void TcpClientConnection::Close()
	{
		closed = true;
		close(socket_fd);
	}

	int TcpClientConnection::Read(char* buf, int buffer_size)
	{
		int bytes_read = recv(socket_fd, buf, buffer_size, 0);
		if (bytes_read <= 0) {
			Close();

			if (bytes_read < 0)
				throw SocketException("An error occurred when reading");
		}
		return bytes_read;
	}

	void TcpClientConnection::Write(char *buf, int buffer_size)
	{
		int bytes_sent = send(socket_fd, buf, buffer_size, 0);
		if (bytes_sent == -1)
			throw SocketException("Error wrinting buffer to client");
	}

	void TcpClientConnection::SignalEvent()
	{
		signaled = true;
	}

	bool TcpClientConnection::IsSignaled()
	{
		return signaled;
	}

	bool TcpClientConnection::IsClosed()
	{
		return closed;
	}

	int TcpClientConnection::FileDescriptor()
	{
		return socket_fd;
	}

}
