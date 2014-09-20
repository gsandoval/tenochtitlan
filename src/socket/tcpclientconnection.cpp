#include "socket/tcpclientconnection.h"
#include "socket/socketexception.h"
#include <sstream>
#include <memory>
#include <chrono>
#include <uv.h>
#include <iostream>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;

		struct WriteCbPayload
		{
			TcpClientConnection *instance;
			uv_buf_t buffer;
			WriteCbPayload(TcpClientConnection *instance, uv_buf_t buffer) : instance(instance), buffer(buffer)
			{

			}
			~WriteCbPayload()
			{
			}
		};

		TcpClientConnection::TcpClientConnection() : closed(false)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("TcpClientConnection"));

			client = new uv_tcp_t();
			client->data = this;

			uv_tcp_init(uv_default_loop(), client);
		}

		TcpClientConnection::~TcpClientConnection()
		{
			delete client;
		}

		void buffer_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
		{
			buf->base = new char[suggested_size];
			buf->len = suggested_size;
		}

		void buffer_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
		{
			TcpClientConnection *that = (TcpClientConnection*)stream->data;
			if (nread == UV_EOF) {
				that->Close();
			} else {
				that->DoRead(buf, nread);
			}
		}

		void TcpClientConnection::DoRead(const uv_buf_t* buf, ssize_t nread)
		{
			if (nread < 0) {
				delete buf->base;
				throw SocketException("An error occurred when reading");
			} else if (nread > 0) {
				shared_ptr<Buffer> buffer(new Buffer(buf->base, nread));

				unique_lock<mutex> lk(read_queue_mutex);
				read_queue.push(buffer);
				lk.unlock();

				read_wait.notify_all();
			} else {
				// NOOP
				delete buf->base;
			}
		}

		void TcpClientConnection::Open(uv_stream_t *server) {
		    if (uv_accept(server, (uv_stream_t*) client) == 0) {
		        uv_read_start((uv_stream_t*) client, buffer_alloc_cb, buffer_read_cb);
		    } else {
		        uv_close((uv_handle_t*) client, NULL);
		        throw SocketException("Could not accept connection");
		    }
		}

		void shutdown_cb(uv_shutdown_t* req, int status)
		{
			delete req;
		}

		void close_cb(uv_handle_t* handle)
		{

		}

		void TcpClientConnection::Close()
		{
			uv_shutdown_t *req = new uv_shutdown_t();
			uv_shutdown(req, (uv_stream_s*)client, shutdown_cb);
			while (uv_is_active((uv_handle_t*)client))
			    this_thread::sleep_for(chrono::milliseconds(100));

			unique_lock<mutex> lk(closed_mutex);
			if (!closed) {
				closed = true;

				uv_close((uv_handle_t*)client, close_cb);
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

		void buffer_written_cb(uv_write_t* req, int status)
		{
			delete req;
		}

		void TcpClientConnection::DoWrite(const uv_buf_t buf)
		{
			uv_write_t *req = new uv_write_t();
			uv_write(req, (uv_stream_t*)client, &buf, 1, buffer_written_cb);
		}

		void process_write_request(uv_async_t* handle)
		{
			WriteCbPayload *cb_payload = (WriteCbPayload*)handle->data;
			cb_payload->instance->DoWrite(cb_payload->buffer);
			delete cb_payload->buffer.base;
			delete cb_payload;
		}

		void TcpClientConnection::Write(char *buf, int buffer_size)
		{
			uv_buf_t buffer;
			buffer.base = buf;
			buffer.len = buffer_size;

			uv_async_t *async = new uv_async_t();
			async->data = new WriteCbPayload(this, buffer);

			uv_async_init(uv_default_loop(), async, process_write_request);
			uv_async_send(async);
		}

		void TcpClientConnection::Write(shared_ptr<Buffer> buf)
		{
			int size = buf->Size();
			char *b = new char[size];

			uv_buf_t buffer;
			buffer.base = b;
			buffer.len = size;

			memcpy(b, buf->Buf(), size);

			uv_async_t *async = new uv_async_t();
			async->data = new WriteCbPayload(this, buffer);

			uv_async_init(uv_default_loop(), async, process_write_request);
			uv_async_send(async);
		}

		void TcpClientConnection::Write(string str)
		{
			int size = str.size() + 1;
			char *b = new char[size];

			uv_buf_t buffer;
			buffer.base = b;
			buffer.len = size;

			strcpy(b, str.c_str());

			uv_async_t *async = new uv_async_t();
			async->data = new WriteCbPayload(this, buffer);

			uv_async_init(uv_default_loop(), async, process_write_request);
			uv_async_send(async);
		}

		bool TcpClientConnection::IsClosed()
		{
			return closed;
		}

	}
}
