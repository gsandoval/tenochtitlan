#include "http/component/restcomponent.h"
#include "http/component/rest/controller.h"

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
				vector<string> path_tokens = Split(resource_path, '/');
				
				bool is_a_match = true;
				map<string, shared_ptr<rest::BaseRoute>> routes = rest::Controller::Routes();
				for (auto it = routes.begin(); it != routes.end(); ++it) {
					vector<string> route_tokens = Split(it->first);
					
					is_a_match = true;
					for (unsigned int i = 0; i < route_tokens.size(); i++) {
						if (route_tokens[i] == "*") {

						} else if (route_tokens[i] == "") {

						}
					}
					if (is_a_match) {
						auto r = it->second;
						r->Execute(ctx);
						break;
					}
				}
				if (!is_a_match) {
					
				}
			}

			void RestComponent::AddController(shared_ptr<rest::Controller> ctrl)
			{
				unique_lock<mutex> lk(controllers_mutex);
				controller_list.push_back(ctrl);
				lk.unlock();
			}

			vector<string> RestComponent::Split(string &str, char sep)
			{
				vector<string> result;
				auto begin = str.find_first_not_of(sep);
				auto end = str.find_first_of(sep, begin);
				result.push_back(str.substr(begin, end));
				while (end != string::npos) {
					begin = str.find_first_not_of(sep, end);
					end = str.find_first_of(sep, begin);
					result.push_back(str.substr(begin, end));
				}
				return result;
			}
		}
	}
}
