#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include "http/httpentity.h"
#include <string>
#include <memory>

namespace tenochtitlan
{
	namespace parser
	{
		class HttpParser
		{
		public:
			std::shared_ptr<http::HttpEntity> Parse(std::string str);
		};
	}
}

#endif
