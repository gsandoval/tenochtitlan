#ifndef _HTTP_FILE_CONTENT_H_
#define _HTTP_FILE_CONTENT_H_

#include "httpcontent.h"
#include "socket/buffer.h"
#include <string>
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace content
		{
			class HttpFileContent : public HttpContent
			{
			public:
				HttpFileContent();
				HttpFileContent(std::shared_ptr<socket::Buffer>);
				void SetMimeForExt(std::string);
			};
		}
	}
}

#endif
