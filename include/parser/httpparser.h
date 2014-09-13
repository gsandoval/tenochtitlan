#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include "http/httpentity.h"
#include "socket/buffer.h"
#include <string>
#include <memory>
#include <vector>
#include <queue>

namespace tenochtitlan
{
	namespace parser
	{
		class HttpParser
		{
		public:
			std::shared_ptr<socket::Buffer> Parse(std::queue<std::shared_ptr<socket::Buffer>> &buffers, std::shared_ptr<http::HttpEntity>);
		};
	}
}

#endif
