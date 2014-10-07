#include "http/component/rest/homecontroller.h"
#include "management/logger.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				using namespace std;

				HomeController::HomeController()
				{
					auto logger = std::shared_ptr<management::Logger>(new management::Logger("HomeController"));
					logger->LogDebug("/home");
					Get<string>("/home", {}, [&]() -> string {
						auto logger = std::shared_ptr<management::Logger>(new management::Logger("HomeController"));
						logger->LogDebug("reached /home");
						return "{'key': 'value'}";
					});
					logger->LogDebug("/phone/{type}");
					Get<string>("/phone/{type}", {}, [&]() -> string {
						auto logger = std::shared_ptr<management::Logger>(new management::Logger("HomeController"));
						logger->LogDebug("reached /phone/{type}");
						return "{'key': 'value'}";
					});
					logger->LogDebug("/address/{type}-{format}");
					Get<string>("/address/{type}-{format}", {}, [&]() -> string {
						auto logger = std::shared_ptr<management::Logger>(new management::Logger("HomeController"));
						logger->LogDebug("reached /address/{type}-{format}");
						return "{'key': 'value'}";
					});
				}
			}
		}
	}
}
