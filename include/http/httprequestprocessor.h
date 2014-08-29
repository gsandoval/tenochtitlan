#ifndef _HTTP_REQUEST_PROCESSOR_H_
#define _HTTP_REQUEST_PROCESSOR_H_

#include "httpentity.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		class HttpRequestProcessor
		{
		public:
			std::shared_ptr<HttpEntity> ProcessRequest(std::shared_ptr<HttpEntity> req);
		};
	}
}

#endif
