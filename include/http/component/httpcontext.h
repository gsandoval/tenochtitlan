#ifndef _HTTP_CONTEXT_H_
#define _HTTP_CONTEXT_H_

#include "http/httpentity.h"
#include "socket/tcpclientconnection.h"
#include "util/properties.h"
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
				std::shared_ptr<socket::TcpClientConnection> connection;
				std::shared_ptr<util::Properties> properties;
			public:
				HttpContext();
				std::shared_ptr<HttpEntity> Request();
				void SetRequest(std::shared_ptr<HttpEntity> req);
				std::shared_ptr<HttpEntity> Response();
				void SetResponse(std::shared_ptr<HttpEntity> res);
				std::shared_ptr<socket::TcpClientConnection> Connection();
				void SetConnection(std::shared_ptr<socket::TcpClientConnection>);
				std::shared_ptr<util::Properties> Properties();
			};
		}
	}
}

#endif
