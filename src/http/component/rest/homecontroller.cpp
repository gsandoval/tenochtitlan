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
					logger->Debug(__func__, "/home");
					Get<string>("/home", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
					logger->Debug(__func__, "/phone/{type}");
					Get<string>("/phone/{type}", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
					logger->Debug(__func__, "/address/{type}-{format}");
					Get<string>("/address/{type}-{format}", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
				}
			}
		}
	}
}
