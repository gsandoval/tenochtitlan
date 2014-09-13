#ifndef _INPUT_VALIDATION_COMPONENT_H_
#define _INPUT_VALIDATION_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class InputValidationComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
			public:
				InputValidationComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
