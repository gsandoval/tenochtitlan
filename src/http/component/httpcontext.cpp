#include "http/component/httpcontext.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			shared_ptr<HttpEntity> HttpContext::Request()
			{
				return request;
			}

			void HttpContext::SetRequest(shared_ptr<HttpEntity> req)
			{
				request = req;
			}

			shared_ptr<HttpEntity> HttpContext::Response()
			{
				return response;
			}

			void HttpContext::SetResponse(shared_ptr<HttpEntity> res)
			{
				response = res;
			}
		}
	}
}
