#include "http/component/httpcontext.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			HttpContext::HttpContext()
			{
				properties = make_shared<util::Properties>();
			}

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

			void HttpContext::SetConnection(shared_ptr<socket::TcpClientConnection> connection)
			{
				this->connection = connection;
			}

			shared_ptr<socket::TcpClientConnection> HttpContext::Connection()
			{
				return connection;
			}

			shared_ptr<util::Properties> HttpContext::Properties()
			{
				return properties;
			}
		}
	}
}
