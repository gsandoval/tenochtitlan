
#ifndef _TCP_CLIENT_CONNECTION_H_
#define _TCP_CLIENT_CONNECTION_H_

#include "management/logger.h"
#include "buffer.h"
#include <ev.h>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace tenochtitlan
{
	namespace socket
	{
		class TcpClientConnection
		{
		private:
			bool closed;
			int socket_fd;
			ev_io *io;
			struct ev_loop *loop;
			std::queue<std::shared_ptr<Buffer>> write_queue;
			std::queue<std::shared_ptr<Buffer>> read_queue;
			std::mutex read_queue_mutex;
			std::mutex write_queue_mutex;
			std::mutex closed_mutex;
			std::condition_variable read_wait;
			std::shared_ptr<management::Logger> logger;

			void UpdateEvents();
		public:
			TcpClientConnection();
			~TcpClientConnection();

			int FileDescriptor();
			void Open(int master_socket);
			void Close();
			std::queue<std::shared_ptr<Buffer>> Read();
			std::queue<std::shared_ptr<Buffer>> ReadOrWait(int time_in_millis);
			void Write(char *buf, int buffer_size);
			bool DoRead();
			void DoWrite();
			void SignalEvent(int socket_fd, int revents);
			bool IsClosed();
			void RequeueBuffer(std::shared_ptr<Buffer>);

			friend void native_callback(struct ev_loop *loop, ev_io *w, int revents);
		};
	}
}

#endif
