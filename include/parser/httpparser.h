#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include "http/httpentity.h"
#include <string>
#include <memory>
#include <vector>

namespace tenochtitlan
{
	namespace parser
	{
		class HttpParser
		{
		private:
			std::unique_ptr<std::vector<std::string>> Split(std::string str, std::vector<char> delims);
			std::string Trim(std::string str);
		public:
			std::shared_ptr<http::HttpEntity> Parse(std::string str);
		};
	}
}

#endif
