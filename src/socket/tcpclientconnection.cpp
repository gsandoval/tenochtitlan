#include "socket/tcpclientconnection.h"
#include "socket/socketexception.h"
#include <sstream>
#include <memory>
#include <chrono>
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
			logger = shared_ptr<management::Logger>(new management::Logger("TcpClientConnection"));
		}

		TcpClientConnection::~TcpClientConnection()
		{
			ostringstream oss;
			oss << socket_fd << " ~TcpClientConnection";
			logger->Debug(__func__, oss.str());
		}

		void TcpClientConnection::Open(int master_socket) {
			struct sockaddr_in clientaddr;
			socklen_t addrlen = sizeof(clientaddr);
			socket_fd = ::accept(master_socket, (struct sockaddr *)&clientaddr, &addrlen);
			if (socket_fd == -1)
				throw SocketException("Could not accept connection");
			//printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), socket_fd);

			ostringstream oss;
			oss << "socket_fd " << socket_fd;
			logger->Debug(__func__, oss.str());

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

		queue<shared_ptr<Buffer>> TcpClientConnection::Read()
		{
			unique_lock<mutex> lk(read_queue_mutex);
			auto local_copy = read_queue;
			while (!read_queue.empty()) read_queue.pop();
			lk.unlock();
			return local_copy;
		}

		queue<shared_ptr<Buffer>> TcpClientConnection::ReadOrWait(int time_in_millis)
		{
			logger->Debug(__func__, "Reading");
			unique_lock<mutex> lk(read_queue_mutex);
			if (read_queue.empty()) {
				logger->Debug(__func__, "waiting");
				read_wait.wait_for(lk, chrono::milliseconds(time_in_millis));
			}
			auto local_copy = read_queue;
			while (!read_queue.empty()) read_queue.pop();
			lk.unlock();
			logger->Debug(__func__, "returning buffer queue");
			return local_copy;
		}

		void TcpClientConnection::Write(char *buf, int buffer_size)
		{
			shared_ptr<Buffer> buffer(new Buffer(buf, buffer_size));
			unique_lock<mutex> lk(write_queue_mutex);
			write_queue.push(buffer);
			lk.unlock();

			UpdateEvents();
		}

		void TcpClientConnection::SignalEvent(ev::io &watcher, int revents)
		{
			if (revents & EV_READ) 
                DoRead();
            if (revents & EV_WRITE) 
                DoWrite();
		}

		bool TcpClientConnection::IsClosed()
		{
			return closed;
		}

		int TcpClientConnection::FileDescriptor()
		{
			return socket_fd;
		}

		void TcpClientConnection::DoRead()
		{
			int buffer_size = 255;
			char *buf = new char[256];
			int bytes_read = recv(socket_fd, buf, buffer_size, 0);
			if (bytes_read <= 0) {
				if (errno == EAGAIN) {
					bytes_read = 0;
				}
				if (bytes_read < 0) {
					throw SocketException("An error occurred when reading");
				}
			}

			shared_ptr<Buffer> buffer(new Buffer(buf, bytes_read));

			unique_lock<mutex> lk(read_queue_mutex);
			read_queue.push(buffer);
			lk.unlock();

			ostringstream oss;
			oss << socket_fd << " Notifying read " << bytes_read << "bytes";
			logger->Debug(__func__, oss.str());

			read_wait.notify_all();
			UpdateEvents();
		}

		void TcpClientConnection::DoWrite()
		{
			unique_lock<mutex> lk(write_queue_mutex);
			while (!write_queue.empty()) {
				shared_ptr<Buffer> buffer = write_queue.front();
				write_queue.pop();
				int bytes_sent = send(socket_fd, buffer->Buf(), buffer->Size(), 0);
				if (bytes_sent == -1)
					throw SocketException("Error writing buffer to client");
			}
			lk.unlock();

			UpdateEvents();
		}

		void TcpClientConnection::UpdateEvents()
		{
			if (write_queue.empty()) {
                io.set(ev::READ);
            } else {
                io.set(ev::READ|ev::WRITE);
            }
		}
	}
}
