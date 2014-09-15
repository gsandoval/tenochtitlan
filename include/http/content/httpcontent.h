#ifndef _HTTP_CONTENT_H_
#define _HTTP_CONTENT_H_

#include "socket/buffer.h"
#include <map>
#include <string>
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace content
		{
			class HttpContent
			{
			protected:
				static std::map<std::string, std::string> MIME_TYPES;
				std::shared_ptr<socket::Buffer> buffer;
				std::string mime;
			public:
				HttpContent();
				HttpContent(std::shared_ptr<socket::Buffer> buffer);
				void SetMime(std::string);
				std::string Mime();
				void SetBuffer(std::shared_ptr<socket::Buffer>);
				std::shared_ptr<socket::Buffer> Buffer();
			};
		}
	}
}

#endif
