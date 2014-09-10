#include "http/component/httpcomponentfactory.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			void HttpComponentFactory::RegisterBuilder(string name, function< shared_ptr<HttpComponent>() > builder)
			{
				builders[name] = builder;
			}

			shared_ptr<HttpComponent> HttpComponentFactory::CreateComponent(string name)
			{
				if (builders.find(name) != builders.end()) {
					return builders[name]();
				}
				return nullptr;
			}
		}
	}
}
