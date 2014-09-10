#ifndef _HTTP_CONTEXT_H_
#define _HTTP_CONTEXT_H_

#include "http/httpentity.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class HttpContext
			{
			private:
				std::shared_ptr<HttpEntity> request;
				std::shared_ptr<HttpEntity> response;
			public:
				std::shared_ptr<HttpEntity> Request();
				void SetRequest(std::shared_ptr<HttpEntity> req);
				std::shared_ptr<HttpEntity> Response();
				void SetResponse(std::shared_ptr<HttpEntity> res);
			};
		}
	}
}

#endif
