#ifndef _STATIC_RESOURCE_COMPONENT_H_
#define _STATIC_RESOURCE_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class StaticResourceComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
				std::string base_path;

				bool Exists(std::string path);
			public:
				StaticResourceComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
				void SetBasePath(std::string base_path);
			};
		}
	}
}

#endif
