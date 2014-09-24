#include "http/component/restcomponent.h"
#include "http/component/rest/controller.h"
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
				vector<string> path_tokens = Split(resource_path, "/");
				
				bool is_a_match = true;
				map<string, shared_ptr<rest::BaseRoute>> routes = rest::Controller::Routes();
				for (auto it = routes.begin(); it != routes.end(); ++it) {
					vector<string> route_tokens = Split(it->first, "/");
					
					is_a_match = true;
					for (unsigned int i = 0; i < route_tokens.size(); i++) {
						if (ContainsVariables(route_tokens[i])) {
							vector<RouteParam> route_params = GetRouteParamTokens(route_tokens[i]);
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

			bool RestComponent::ContainsVariables(string &str)
			{
				auto open = str.find_first_of('{');
				return open != string::npos && str.find_first_of("}", open) != string::npos;
			}

			vector<RouteParam> RestComponent::GetRouteParamTokens(string &str)
			{
				vector<RouteParam> params;
				int last_end = 0;
				int begin = 0;
				int end = 0;
				while (end != string::npos) {
					begin = str.find_first_of("{", end);
					end = str.find_first_of("}", begin);
					if (begin != last_end) {
						RouteParam rp;
						rp.is_separator = true;
						rp.name = str.substr(last_end, begin);
					}
					last_end = end + 1;
					string token = str.substr(begin + 1, end - 1);
					string name = token;
					string type = "string";
					int colon = token.find_first_of(':');
					if (colon != string::npos) {
						name = token.substr(0, colon);
						type = token.substr(colon + 1, token.size() - colon - 1);
					}
					RouteParam param;
					param.is_separator = false;
					param.name = name;
					if (type == "string") param.type = ParamType::String;
					else if (type == "double") param.type = ParamType::Double;
					else if (type == "float") param.type = ParamType::Float;
					else if (type == "bool") param.type = ParamType::Bool;
					else if (type == "int") param.type = ParamType::Int;
					else if (type == "regex") param.type = ParamType::Regex;

					params.push_back(param);
				}

				return params;
			}

			void RestComponent::AddController(shared_ptr<rest::Controller> ctrl)
			{
				unique_lock<mutex> lk(controllers_mutex);
				controller_list.push_back(ctrl);
				lk.unlock();
			}

			vector<string> RestComponent::Split(const string &str, const string &sep)
			{
				vector<string> result;
				int begin = 0;
				int end = 0;
				result.push_back(str.substr(begin, end));
				while (end != string::npos) {
					begin = str.find_first_not_of(sep, end);
					end = str.find_first_of(sep, begin);
					if (begin != string::npos)
						result.push_back(str.substr(begin, end - begin));
				}
				return result;
			}
		}
	}
}
