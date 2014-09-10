#ifndef _HTTP_REQUEST_PROCESSOR_H_
#define _HTTP_REQUEST_PROCESSOR_H_

#include "httpentity.h"
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
			std::shared_ptr<HttpEntity> ProcessRequest(std::shared_ptr<HttpEntity> req);
			void AddComponent(std::shared_ptr<component::HttpComponent> component);
		};
	}
}

#endif
