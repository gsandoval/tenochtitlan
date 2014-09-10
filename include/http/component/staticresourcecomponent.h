#ifndef _STATIC_RESOURCE_COMPONENT_H_
#define _STATIC_RESOURCE_COMPONENT_H_

#include "httpcomponent.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class StaticResourceComponent : public HttpComponent
			{
			public:
				void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
