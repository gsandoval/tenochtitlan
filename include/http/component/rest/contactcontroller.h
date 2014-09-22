#ifndef _CONTACT_CONTROLLER_H_
#define _CONTACT_CONTROLLER_H_

#include "http/component/rest/controller.h"

namespace http
{
	namespace component
	{
		namespace rest
		{
			class ContactController : public http::component::rest::Controller
			{
			public:
				ContactController();
			};
		}
	}
}

#endif
