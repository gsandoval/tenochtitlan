#ifndef _HTTP_ENTITY_H_
#define _HTTP_ENTITY_H_

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
		};
	}
}

#endif
