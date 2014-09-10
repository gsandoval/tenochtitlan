#ifndef _REST_COMPONENT_H_
#define _REST_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class RestComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
			public:
				RestComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
