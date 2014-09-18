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
			char *buffer;
			WriteCbPayload(TcpClientConnection *instance, char *buffer) : instance(instance), buffer(buffer)
			{

			}
			~WriteCbPayload()
			{
				delete buffer;
			}
		};

		TcpClientConnection::TcpClientConnection() : closed(false), write_count(0)
		{
			logger = shared_ptr<management::Logger>(new management::Logger("TcpClientConnection"));

			loop = new uv_loop_t();
			uv_loop_init(loop);

			client = new uv_tcp_t();
			client->data = this;

			uv_tcp_init(loop, client);
		}

		TcpClientConnection::~TcpClientConnection()
		{
			delete client;
			delete loop;
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
				logger->Debug(__func__, buf->base);
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

			thread t = thread([&] {
				uv_run(loop, UV_RUN_DEFAULT);
			});
			t.detach();
		}

		void TcpClientConnection::Close()
		{
			logger->Debug(__func__, "Closing socket!");

			/*
			while (WriteCount() > 0) {
			    this_thread::sleep_for(chrono::milliseconds(100));
			}
			*/

			logger->Debug(__func__, "After sleep");

			unique_lock<mutex> lk(closed_mutex);
			if (!closed) {
				closed = true;

				uv_shutdown_t req;
				uv_shutdown(&req, (uv_stream_t*)client, NULL);

				logger->Debug(__func__, "Before close");
				uv_close((uv_handle_t*)client, NULL);
				logger->Debug(__func__, "Before stop");
				uv_stop(loop);
				logger->Debug(__func__, "Before loop close");
				uv_loop_close(loop);
			}
			lk.unlock();
			logger->Debug(__func__, "After close unlock");
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
			cout << "Testinginginging" << endl;
			WriteCbPayload* cb_payload = (WriteCbPayload*)req->data;
			cb_payload->instance->DecreaseWriteCount();
			delete cb_payload;
			delete req;
		}

		void TcpClientConnection::Write(char *buf, int buffer_size)
		{
			uv_buf_t buffer[1];
			buffer[0].base = buf;
			buffer[0].len = buffer_size;

			auto cb_payload = new WriteCbPayload(this, buf);
			IncreaseWriteCount();

			uv_write_t *req = new uv_write_t();
			req->data = cb_payload;

			uv_write(req, (uv_stream_t*)client, buffer, 1, NULL);
		}

		void TcpClientConnection::Write(shared_ptr<Buffer> buf)
		{
			int size = buf->Size();
			char *b = new char[size];

			uv_buf_t buffer[1];
			buffer[0].base = b;
			buffer[0].len = size;

			ostringstream oss;
			oss << "Queuing " << size;
			logger->Debug(__func__, oss.str());

			memcpy(b, buf->Buf(), size);

			auto cb_payload = new WriteCbPayload(this, b);
			IncreaseWriteCount();

			uv_write_t *req = new uv_write_t();
			req->data = cb_payload;

			uv_write(req, (uv_stream_t*)client, buffer, 1, NULL);
		}

		void TcpClientConnection::Write(string str)
		{
			int size = str.size() + 1;
			char *b = new char[size];

			uv_buf_t buffer[1];
			buffer[0].base = b;
			buffer[0].len = size;

			strcpy(b, str.c_str());

			auto cb_payload = new WriteCbPayload(this, b);
			IncreaseWriteCount();

			uv_write_t *req = new uv_write_t();
			req->data = cb_payload;

			uv_write(req, (uv_stream_t*)client, buffer, 1, NULL);
		}

		bool TcpClientConnection::IsClosed()
		{
			return closed;
		}

		void TcpClientConnection::IncreaseWriteCount()
		{
			logger->Debug(__func__, "Before increase");
			unique_lock<mutex> lk(write_count_mutex);
			write_count++;
			ostringstream oss;
			oss << "Increasing " << write_count;
			logger->Debug(__func__, oss.str());
			lk.unlock();
		}

		void TcpClientConnection::DecreaseWriteCount()
		{
			logger->Debug(__func__, "Before decrease");
			unique_lock<mutex> lk(write_count_mutex);
			write_count--;
			ostringstream oss;
			oss << "Decreasing " << write_count;
			logger->Debug(__func__, oss.str());
			lk.unlock();
		}

		int TcpClientConnection::WriteCount()
		{
			unique_lock<mutex> lk(write_count_mutex);
			int local = write_count;
			lk.unlock();
			return local;
		}

	}
}
