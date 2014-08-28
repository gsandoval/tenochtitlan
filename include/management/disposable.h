#ifndef _DISPOSABLE_H_
#define _DISPOSABLE_H_

namespace tenochtitlan
{
	namespace management
	{
		class Disposable
		{
		public:
			virtual void Dispose() = 0;
		};
	}
}

#endif
