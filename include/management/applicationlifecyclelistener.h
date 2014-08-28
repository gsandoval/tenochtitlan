#ifndef _APPLICATION_LIFECYCLE_LISTENER_H_
#define _APPLICATION_LIFECYCLE_LISTENER_H_

#include "disposable.h"

#include <memory>
#include <vector>

namespace tenochtitlan
{
	namespace management
	{
		void ctrl_c_handler(int);

		class ApplicationLifecycleListener
		{
		private:
			std::vector<std::shared_ptr<Disposable>> global_pool;

			void Shutdown();
		public:
			ApplicationLifecycleListener();
			static ApplicationLifecycleListener* Instance();

			void AddToGlobalPool(std::shared_ptr<Disposable> disposable);

			friend void ctrl_c_handler(int);
		};
	}
}

#endif
