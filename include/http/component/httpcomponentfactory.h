#ifndef _HTTP_COMPONENT_FACTORY_H_
#define _HTTP_COMPONENT_FACTORY_H_

#include "httpcomponent.h"
#include <map>
#include <string>
#include <functional>
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class HttpComponentFactory
			{
			private:
				std::map<std::string, std::function< std::shared_ptr<HttpComponent>() >> builders;
			public:
				void RegisterBuilder(std::string name, std::function< std::shared_ptr<HttpComponent>() >);
				std::shared_ptr<HttpComponent> CreateComponent(std::string name);
			};
		}
	}
}

#endif
