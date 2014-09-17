#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>
#include <memory>

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
			Buffer(std::string);
			~Buffer();

			char* Buf();
			int Size();
			void Append(std::shared_ptr<Buffer>);
		};
	}
}	

#endif
