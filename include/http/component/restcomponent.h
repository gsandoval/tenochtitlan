#ifndef _REST_COMPONENT_H_
#define _REST_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include "http/component/rest/controller.h"
#include <memory>
#include <vector>
#include <mutex>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			class RestComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
				std::vector<std::shared_ptr<rest::Controller>> controller_list;
				std::mutex controllers_mutex;
			public:
				RestComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
				void AddController(std::shared_ptr<rest::Controller> ctrl);
			};
		}
	}
}

#endif
