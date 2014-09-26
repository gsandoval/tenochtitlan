#include "http/component/restcomponent.h"
#include "http/component/rest/controller.h"
#include "util/stringutils.h"
#include <algorithm>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			RestComponent::RestComponent()
			{
				logger = shared_ptr<management::Logger>(new management::Logger("RestComponent"));
			}

			void RestComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				auto props = ctx->Properties();
				if (!props->GetBool("t:IsValid") || props->GetBool("t:IsHandled"))
					return;

				auto req = ctx->Request();
				auto res = ctx->Response();

				string resource_path = req->ResourcePath();
				logger->Debug(__func__, resource_path);
				vector<string> actual_path_tokens = util::StringUtils::Split(resource_path, "/");

				bool is_a_match = true;
				bool wrong_method = false;
				map<string, shared_ptr<rest::BaseRoute>> routes = rest::Controller::Routes();
				for (auto it = routes.begin(); it != routes.end(); ++it) {
					shared_ptr<rest::BaseRoute> route = it->second;
					
					is_a_match = false;
					if (route->route_tokens.size() == actual_path_tokens.size()) {
						is_a_match = true;
						for (unsigned int i = 0; i < route->route_tokens.size(); i++) {
							string &actual_token = actual_path_tokens[i];
							if (route->contains_variables[i]) {
								vector<shared_ptr<rest::RouteParam>> route_params = route->route_params[i];
								vector<string> token_variables = route->SeparateTokenVariables(actual_token, route_params);
								for (int j = 0; j < token_variables.size(); j++) {
									logger->Debug(__func__, token_variables[j]);
								}
								int current = 0;
								for (unsigned int j = 0; j < route_params.size(); j++) {

								}
							} else if (route->route_tokens[i] == actual_token) {
								// NOOP
							} else {
								is_a_match = false;
								break;
							}
						}
					}

					auto r = it->second;
					if (is_a_match && r->method == req->Method()) {
						r->Execute(ctx);
						wrong_method = false;

						props->Set("t:IsHandled", true);
						props->Set("t:HandledBy", "rest_resource_component");

						res->SetCode(200);
						res->SetCodeStr("OK");

						break;
					} else if (is_a_match) {
						wrong_method = true;
					}
				}
				if (!is_a_match) {
					
				} else if (wrong_method) {

				}
			}

			void RestComponent::AddController(shared_ptr<rest::Controller> ctrl)
			{
				unique_lock<mutex> lk(controllers_mutex);
				controller_list.push_back(ctrl);
				lk.unlock();
			}
		}
	}
}
