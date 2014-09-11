#ifndef _HTTP_REQUEST_PROCESSOR_H_
#define _HTTP_REQUEST_PROCESSOR_H_

#include "component/httpcontext.h"
#include "component/httpcomponentfactory.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		class HttpRequestProcessor
		{
		private:
			std::vector<std::shared_ptr<component::HttpComponent>> components;
		public:
			void ProcessRequest(std::shared_ptr<component::HttpContext> req);
			void AddComponent(std::shared_ptr<component::HttpComponent> component);
		};
	}
}

#endif
