#ifndef _REST_COMPONENT_H_
#define _REST_COMPONENT_H_

#include "httpcomponent.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class RestComponent : public HttpComponent
			{
			public:
				void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
