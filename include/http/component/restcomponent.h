#ifndef _REST_COMPONENT_H_
#define _REST_COMPONENT_H_

#include "httpcomponent.h"
#include "management/logger.h"
#include "http/component/rest/controller.h"
#include <memory>
#include <vector>
#include <mutex>
#include <string>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			enum ParamType
			{
				Regex, Int, Double, Float, String, Bool
			};

			struct RouteParam
			{
				bool is_separator;
				ParamType type;
				std::string name;
			};

			class RestComponent : public HttpComponent
			{
			private:
				std::shared_ptr<management::Logger> logger;
				std::vector<std::shared_ptr<rest::Controller>> controller_list;
				std::mutex controllers_mutex;

				std::vector<std::string> Split(const std::string &str, const std::string &sep);
				bool ContainsVariables(std::string &str);
				std::vector<RouteParam> GetRouteParamTokens(std::string &str);
			public:
				RestComponent();
				void Execute(std::shared_ptr<HttpContext> ctx);
				void AddController(std::shared_ptr<rest::Controller> ctrl);
			};
		}
	}
}

#endif
