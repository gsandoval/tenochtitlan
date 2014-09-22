#ifndef _HOME_CONTROLLER_H_
#define _HOME_CONTROLLER_H_

#include "http/component/rest/controller.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				class HomeController : public tenochtitlan::http::component::rest::Controller
				{
				public:
					HomeController();
				};
			}
		}
	}
}

#endif
