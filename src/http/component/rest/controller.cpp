#include "http/component/rest/controller.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				using namespace std;

				mutex Controller::routes_mutex;
				map<string, shared_ptr<BaseRoute>> Controller::routes;
			}
		}
	}
}

