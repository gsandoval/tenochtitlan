#include "socket/tcpclientconnection.h"
#include "socket/socketexception.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		TcpClientConnection::TcpClientConnection() : closed(false)
		{
		}

		TcpClientConnection::~TcpClientConnection()
		{
			cout << "~TcpClientConnection" << endl;
		}

		void TcpClientConnection::Open(int master_socket) {
			struct sockaddr_in clientaddr;
			socklen_t addrlen = sizeof(clientaddr);
			socket_fd = ::accept(master_socket, (struct sockaddr *)&clientaddr, &addrlen);
			if (socket_fd == -1)
				throw SocketException("Could not accept connection");
			//printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), socket_fd);

			cout << "socket_fd" << socket_fd << endl;
			fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK); 
            io.set<TcpClientConnection, &TcpClientConnection::SignalEvent>(this);

            io.start(socket_fd, ev::READ);
		}

		void TcpClientConnection::Close()
		{
			closed = true;
			io.stop();
			close(socket_fd);
		}

		int TcpClientConnection::Read(char* buf, int buffer_size)
		{
			int bytes_read = recv(socket_fd, buf, buffer_size, 0);
			if (bytes_read <= 0) {
				if (bytes_read == 0) {
					Close();
				}
				if (errno == EAGAIN) {
					bytes_read = 0;
				}
				if (bytes_read < 0) {
					throw SocketException("An error occurred when reading");
				}
			}
			return bytes_read;
		}

		void TcpClientConnection::Write(char *buf, int buffer_size)
		{
			int bytes_sent = send(socket_fd, buf, buffer_size, 0);
			if (bytes_sent == -1)
				throw SocketException("Error writing buffer to client");
		}

		void TcpClientConnection::SignalEvent(ev::io &watcher, int revents)
		{
			if (revents & EV_READ) 
                DoRead();

            if (revents & EV_WRITE) 
                DoWrite();

            io.set(ev::READ);
            /*
            if (write_queue.empty()) {
                io.set(ev::READ);
            } else {
                io.set(ev::READ|ev::WRITE);
            }
            */
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
}
