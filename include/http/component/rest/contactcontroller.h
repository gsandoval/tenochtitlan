#ifndef _CONTACT_CONTROLLER_H_
#define _CONTACT_CONTROLLER_H_

#include "http/component/rest/controller.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				class ContactController : public tenochtitlan::http::component::rest::Controller
				{
				public:
					ContactController();
				};
			}
		}
	}
}

#endif
