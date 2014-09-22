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
					Get<string>("/", {}, [&]() -> string {
						return "";
					});
				}
			}
		}
	}
}
