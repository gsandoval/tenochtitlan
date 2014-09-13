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
			loop = ev_loop_new(EVFLAG_AUTO);
			ev_set_userdata(loop, this);
			io = new ev_io();
		}

		TcpClientConnection::~TcpClientConnection()
		{
			delete io;
			ev_loop_destroy(loop);
		}

		void native_callback(struct ev_loop *loop, ev_io *w, int revents)
		{
			void *data = ev_userdata(loop);
			TcpClientConnection *conn = (TcpClientConnection *)data;
			conn->SignalEvent(w->fd, revents);
		}

		void TcpClientConnection::Open(int master_socket) {
			struct sockaddr_in clientaddr;
			socklen_t addrlen = sizeof(clientaddr);
			socket_fd = ::accept(master_socket, (struct sockaddr *)&clientaddr, &addrlen);
			if (socket_fd == -1)
				throw SocketException("Could not accept connection");
			//printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), socket_fd);

			fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);

			ev_io_init(io, native_callback, socket_fd, EV_READ);
			ev_io_start(loop, io);

			thread t = thread([&] {
				ev_run(loop, 0);
			});
			t.detach();
		}

		void TcpClientConnection::Close()
		{
			unique_lock<mutex> lk(closed_mutex);
			if (!closed) {
				closed = true;

				ev_io_stop(loop, io);

				close(socket_fd);

				ev_break(loop);
			}
			lk.unlock();
		}

		queue<shared_ptr<Buffer>> TcpClientConnection::Read()
		{
			unique_lock<mutex> lk(read_queue_mutex);
			auto local_copy = read_queue;
			while (!read_queue.empty()) read_queue.pop();
			lk.unlock();
			return local_copy;
		}

		void TcpClientConnection::RequeueBuffer(shared_ptr<Buffer> buffer)
		{
			unique_lock<mutex> lk(read_queue_mutex);
			queue<shared_ptr<Buffer>> new_queue;
			new_queue.push(buffer);
			while (!read_queue.empty()) {
				new_queue.push(read_queue.front());
				read_queue.pop();
			}
			read_queue = new_queue;
			lk.unlock();
		}

		queue<shared_ptr<Buffer>> TcpClientConnection::ReadOrWait(int time_in_millis)
		{
			unique_lock<mutex> lk(read_queue_mutex);
			if (read_queue.empty()) {
				read_wait.wait_for(lk, chrono::milliseconds(time_in_millis));
			}
			auto local_copy = read_queue;
			while (!read_queue.empty()) read_queue.pop();
			lk.unlock();
			return local_copy;
		}

		void TcpClientConnection::Write(char *buf, int buffer_size)
		{
			shared_ptr<Buffer> buffer(new Buffer(buf, buffer_size));
			unique_lock<mutex> lk(write_queue_mutex);
			write_queue.push(buffer);
			UpdateEvents(); // Must be sync with the write_queue lock
			lk.unlock();
		}

		void TcpClientConnection::SignalEvent(int socket_fd, int revents)
		{
			unique_lock<mutex> lk(closed_mutex);
			if (closed) {
				logger->Warn(__func__, "Message ignored, connection closed.");
				return;
			}
			bool close_requested = false;
			if (revents & EV_READ) 
                close_requested = DoRead();
            if (revents & EV_WRITE) 
                DoWrite();
            lk.unlock();

            if (close_requested)
            	Close();
		}

		bool TcpClientConnection::IsClosed()
		{
			return closed;
		}

		int TcpClientConnection::FileDescriptor()
		{
			return socket_fd;
		}

		bool TcpClientConnection::DoRead()
		{
			bool close_request = false;
			int buffer_size = 255;
			char *buf = new char[256];
			int bytes_read = recv(socket_fd, buf, buffer_size, 0);
			if (bytes_read <= 0) {
				if (bytes_read == 0 && errno != EAGAIN) {
					close_request = true;
				}
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

			read_wait.notify_all();

			unique_lock<mutex> write_lock(write_queue_mutex);
			UpdateEvents(); // Must be sync with the write_queue lock
			write_lock.unlock();

			return close_request;
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

			UpdateEvents(); // Must be sync with the write_queue lock
			lk.unlock();
		}

		void TcpClientConnection::UpdateEvents()
		{
			if (write_queue.empty()) {
                ev_io_set(io, socket_fd, EV_READ);
            } else {
            	ev_io_set(io, socket_fd, EV_READ | EV_WRITE);
            }
		}
	}
}
