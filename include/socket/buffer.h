#ifndef _BUFFER_H_
#define _BUFFER_H_

namespace tenochtitlan
{
	namespace socket 
	{
		class Buffer
		{
		private:
			char* buf;
			int buffer_size;
		public:
			Buffer();
			Buffer(char *buf, int buffer_size);
			~Buffer();

			char* Buf();
			int Size();
		};
	}
}	

#endif
