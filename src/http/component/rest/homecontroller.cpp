#include "http/component/rest/homecontroller.h"

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
					Get<string>("/home", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
					Get<string>("/phone/{type}", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
					Get<string>("/address/{type}-{format}", {}, [&]() -> string {
						return "{'key': 'value'}";
					});
				}
			}
		}
	}
}
