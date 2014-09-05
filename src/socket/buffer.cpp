#include "socket/buffer.h"

namespace tenochtitlan
{
	namespace socket
	{
		Buffer::Buffer()
		{

		}

		Buffer::Buffer(char *buf, int buffer_size) : buf(buf), buffer_size(buffer_size)
		{

		}

		Buffer::~Buffer()
		{
		}

		char* Buffer::Buf()
		{
			return buf;
		}

		int Buffer::Size()
		{
			return buffer_size;
		}
	}
}
