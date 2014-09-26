#ifndef _HTTP_STRING_CONTENT_H_
#define _HTTP_STRING_CONTENT_H_

#include "httpcontent.h"
#include <string>

namespace tenochtitlan
{
	namespace http
	{
		namespace content
		{
			class HttpStringContent : public HttpContent
			{
			public:
				HttpStringContent();
				HttpStringContent(std::shared_ptr<socket::Buffer>);
				HttpStringContent(std::string content);
				HttpStringContent(std::shared_ptr<socket::Buffer>, std::string mime);
				HttpStringContent(std::string content, std::string mime);
			};
		}
	}
}

#endif
