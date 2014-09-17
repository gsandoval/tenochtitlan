#include "socket/buffer.h"
#include <cstdlib>

namespace tenochtitlan
{
	namespace socket
	{
		using namespace std;
		
		Buffer::Buffer() : buf(0), buffer_size(0)
		{

		}

		Buffer::Buffer(string string_buf)
		{
			buffer_size = string_buf.size();
			buf = new char[buffer_size];
			memcpy(buf, string_buf.c_str(), buffer_size);
		}

		Buffer::Buffer(char *buf, int buffer_size) : buf(buf), buffer_size(buffer_size)
		{

		}

		Buffer::~Buffer()
		{
			if (buf)
				delete buf;
		}

		char* Buffer::Buf()
		{
			return buf;
		}

		int Buffer::Size()
		{
			return buffer_size;
		}

		void Buffer::Append(shared_ptr<Buffer> b)
		{
			int new_size = b->Size() + buffer_size;
			char *new_buffer = new char[new_size];
			memcpy(new_buffer, buf, buffer_size);
			memcpy(new_buffer + buffer_size, b->Buf(), b->Size());

			delete buf;
			buf = new_buffer;
			buffer_size = new_size;
		}
	}
}
