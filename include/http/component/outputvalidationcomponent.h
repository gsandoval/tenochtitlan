#ifndef _OUTPUT_VALIDATION_COMPONENT_H_
#define _OUTPUT_VALIDATION_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include <memory>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class OutputValidationComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
			public:
				OutputValidationComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
			};
		}
	}
}

#endif
