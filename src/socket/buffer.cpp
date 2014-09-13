#include "socket/buffer.h"

namespace tenochtitlan
{
	namespace socket
	{
		Buffer::Buffer() : buf(0), buffer_size(0)
		{

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
	}
}
