
#ifndef _TCP_CLIENT_CONNECTION_H_
#define _TCP_CLIENT_CONNECTION_H_

#include "management/logger.h"
#include "buffer.h"
#include <uv.h>
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
			uv_loop_t *loop;
			uv_tcp_t *client;

			bool closed;
			std::queue<std::shared_ptr<Buffer>> read_queue;
			std::mutex read_queue_mutex;
			std::mutex closed_mutex;
			std::condition_variable read_wait;
			std::shared_ptr<management::Logger> logger;
		public:
			TcpClientConnection();
			~TcpClientConnection();

			void Open(uv_stream_t *server);
			void Close();
			std::queue<std::shared_ptr<Buffer>> Read();
			std::queue<std::shared_ptr<Buffer>> ReadOrWait(int time_in_millis);
			void Write(char *buf, int buffer_size);
			void Write(std::shared_ptr<Buffer> buffer);
			void Write(std::string str);
			void DoRead(const uv_buf_t* buf, ssize_t nread);
			void DoWrite(const uv_buf_t buf);
			bool IsClosed();
			void RequeueBuffer(std::shared_ptr<Buffer>);

			friend void buffer_written_cb(uv_write_t* req, int status);
			friend void buffer_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
			friend void buffer_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
			friend void process_write_request(uv_async_t* handle);
		};
	}
}

#endif
