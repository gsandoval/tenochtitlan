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
#include <netinet/tcp.h>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		TcpClientConnection::TcpClientConnection() : closed(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("TcpClientConnection"));
			loop = ev_loop_new(EVFLAG_AUTO);
			ev_set_userdata(EV_A_ this);
			read_watch = new ev_io();
			write_watch = new ev_io();
		}

		TcpClientConnection::~TcpClientConnection()
		{
			delete read_watch;
			delete write_watch;
			ev_loop_destroy(EV_A);
		}

		void native_callback(EV_P_ ev_io *w, int revents)
		{
			void *data = ev_userdata(EV_A);
			TcpClientConnection *conn = (TcpClientConnection *)data;
			conn->SignalEvent(w->fd, revents);
		}

		void TcpClientConnection::Open(int master_socket) {
			struct sockaddr_in clientaddr;
			socklen_t addrlen = sizeof(clientaddr);
			socket_fd = ::accept(master_socket, (struct sockaddr *)&clientaddr, &addrlen);
			if (socket_fd == -1)
				throw SocketException("Could not accept connection");

			/*
			int flag = 1;
			int result = setsockopt(socket_fd, SOL_SOCKET, TCP_NODELAY, (char *) &flag, sizeof(int));

			
			struct linger linger = { 0 };
			linger.l_onoff = 1;
			linger.l_linger = 30;

			result = setsockopt(socket_fd, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger));
			*/

			fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);

			ev_io_init(read_watch, native_callback, socket_fd, EV_READ);
			ev_io_init(write_watch, native_callback, socket_fd, EV_WRITE);
			ev_io_start(EV_A_ read_watch);

			thread t = thread([&] {
				ev_run(EV_A_ 0);
			});
			t.detach();
		}

		void TcpClientConnection::Close()
		{
			logger->Debug(__func__, "Closing socket!");
			this_thread::sleep_for(chrono::milliseconds(1000));

			unique_lock<mutex> lk(closed_mutex);
			if (!closed) {
				closed = true;

				ev_io_stop(EV_A_ read_watch);
				ev_io_stop(EV_A_ write_watch);

				shutdown(socket_fd, SHUT_RDWR);
				close(socket_fd);

				ev_break(EV_A);
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

			//DoWrite(); // FIXME: WRONG WRONG WRONG Why is the write event not being called from libev
		}

		void TcpClientConnection::Write(shared_ptr<Buffer> buffer)
		{
			unique_lock<mutex> lk(write_queue_mutex);
			write_queue.push(buffer);
			UpdateEvents(); // Must be sync with the write_queue lock
			lk.unlock();

			//DoWrite(); // FIXME: WRONG WRONG WRONG Why is the write event not being called from libev
		}

		void TcpClientConnection::Write(string str)
		{
			shared_ptr<Buffer> buffer(new Buffer(str));
			unique_lock<mutex> lk(write_queue_mutex);
			write_queue.push(buffer);
			UpdateEvents(); // Must be sync with the write_queue lock
			lk.unlock();

			//DoWrite(); // FIXME: WRONG WRONG WRONG Why is the write event not being called from libev
		}

		void TcpClientConnection::SignalEvent(int socket_fd, int revents)
		{
			unique_lock<mutex> lk(closed_mutex);
			if (closed) {
				logger->Warn(__func__, "Message ignored, connection closed.");
				return;
			}
			bool close_requested = false;
			if (revents & EV_WRITE) 
                DoWrite();
			if (revents & EV_READ) 
                close_requested = DoRead();
            lk.unlock();

            if (close_requested) {
            	logger->Warn(__func__, "Closing socket due to a false read operation");
            	Close();
            }
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
			logger->Debug(__func__, "Writing...");
			unique_lock<mutex> lk(write_queue_mutex);
			while (!write_queue.empty()) {
				shared_ptr<Buffer> buffer = write_queue.front();
				write_queue.pop();

				char *buf = buffer->Buf();
				int left = buffer->Size();

				while (left > 0) {
					int bytes_sent = send(socket_fd, buf, left, 0);
					left -= bytes_sent;
					buf += bytes_sent;

					ostringstream oss;
					oss << "bytes sent " << bytes_sent;
					logger->Debug(__func__, oss.str());

					if (bytes_sent == -1) {
						oss = ostringstream();
						oss << "Error sending data, errno: " << errno;
						logger->Error(__func__, oss.str());
						throw SocketException("Error writing buffer to client");
					}
				}
			}

			UpdateEvents(); // Must be sync with the write_queue lock
			lk.unlock();
		}

		void TcpClientConnection::UpdateEvents()
		{
			if (write_queue.empty()) {
				ev_io_stop(EV_A_ write_watch);
                //ev_io_set(io, socket_fd, EV_READ);
            } else {
            	logger->Debug(__func__, "Requesting write");
            	ev_io_start(EV_A_ write_watch);
            	//ev_io_set(io, socket_fd, EV_READ | EV_WRITE);
            }
		}
	}
}
