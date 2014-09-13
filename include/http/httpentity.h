#ifndef _HTTP_ENTITY_H_
#define _HTTP_ENTITY_H_

#include "socket/buffer.h"
#include <string>
#include <utility>
#include <vector>

namespace tenochtitlan
{
	namespace http
	{
		class HttpEntity
		{
		private:
			std::string method;
			std::string resource_path;
			std::string version;
			std::string host;
			std::vector<std::pair<std::string, std::string>> headers;
			std::shared_ptr<socket::Buffer> content;
			int code;
			std::string code_str;
		public:
			std::string Method();
			void SetMethod(std::string);
			std::string Version();
			void SetVersion(std::string);
			std::string Host();
			void SetHost(std::string);
			std::string ResourcePath();
			void SetResourcePath(std::string);
			std::string Header(std::string name);
			void AddHeader(std::string name, std::string value);
			void SetContent(std::string);
			void SetContent(std::shared_ptr<socket::Buffer> buffer);
			std::shared_ptr<socket::Buffer> ContentAsBuffer();
			std::string ContentAsString();
			std::vector<std::string> HeaderNames();
			int Code();
			void SetCode(int code);
			std::string CodeStr();
			void SetCodeStr(std::string code_str);
		};
	}
}

#endif
