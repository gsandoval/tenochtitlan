#ifndef _HTTP_COMPONENT_H_
#define _HTTP_COMPONENT_H_

#include "httpcontext.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class HttpComponent
			{
			private:
				std::shared_ptr<HttpComponent> next_component;
			public:
				void SetNext(std::shared_ptr<HttpComponent>);
				virtual void Next(std::shared_ptr<HttpContext> ctx);
				virtual void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
