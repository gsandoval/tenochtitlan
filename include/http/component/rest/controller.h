#ifndef _REST_CONTROLLER_H_
#define _REST_CONTROLLER_H_

#include "http/component/httpcontext.h"
#include "util/stringutils.h"
#include "http/content/httpstringcontent.h"
#include "management/logger.h"
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <mutex>
#include <iostream>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				enum ParamType
				{
					Regex, Int, Double, String, Bool
				};

				struct RouteParam
				{
					bool is_separator;
					ParamType type;
					std::string name;
				};

				template <typename T>
				struct identity
				{
					typedef T type;
				};

				struct BaseRoute
				{
					std::string method;
					std::vector<std::string> param_names;
					std::vector<std::string> route_tokens;
					std::vector<bool> contains_variables;
					std::string route;
					std::vector<std::vector<std::shared_ptr<RouteParam>>> route_params;

					BaseRoute(std::string route) : route(route)
					{
						route_tokens = util::StringUtils::Split(route, "/");

						bool contains;
						for (unsigned int i = 0; i < route_tokens.size(); i++) {
							contains = ContainsVariables(route_tokens[i]);
							contains_variables.push_back(contains);
							if (contains) {
								route_params.push_back(GetRouteParamTokens(route_tokens[i]));
							} else {
								route_params.push_back(std::vector<std::shared_ptr<RouteParam>>());
							}
						}
					}

					virtual void Execute(std::shared_ptr<http::component::HttpContext> ctx) = 0;

					std::vector<std::string> SeparateTokenVariables(std::string &str, std::vector<std::shared_ptr<rest::RouteParam>> params)
					{
						int begin = 0;
						int end = 0;
						std::vector<std::string> tv;
						for (unsigned int i = 0; i < params.size(); i++) {
							if (params[i]->is_separator) {
								end = str.find_first_of(params[i]->name, begin);
								if (begin != end) {
									tv.push_back(str.substr(begin, end - begin));
								}
								begin = end + params[i]->name.size();
							}
						}
						if (end < str.size() - 1) {
							tv.push_back(str.substr(end, str.size() - end));
						}
						return tv;
					}

					bool ContainsVariables(std::string &str)
					{
						auto open = str.find_first_of('{');
						return open != std::string::npos && str.find_first_of("}", open) != std::string::npos;
					}

					std::vector<std::shared_ptr<RouteParam>> GetRouteParamTokens(std::string &str)
					{
						std::vector<std::shared_ptr<RouteParam>> params;
						int last_end = 0;
						int begin = 0;
						int end = 0;
						while (end != std::string::npos) {
							begin = str.find_first_of("{", end);
							end = str.find_first_of("}", begin);
							if (begin != last_end) {
								auto rp = std::make_shared<RouteParam>();
								rp->is_separator = true;
								rp->name = str.substr(last_end, begin);
								params.push_back(rp);
							}
							last_end = end + 1;
							std::string token = str.substr(begin + 1, end - 1);
							std::string name = token;
							std::string type = "string";
							int colon = token.find_first_of(':');
							if (colon != std::string::npos) {
								name = token.substr(0, colon);
								type = token.substr(colon + 1, token.size() - colon - 1);
							}
							auto param = std::make_shared<RouteParam>();
							param->is_separator = false;
							param->name = name;
							if (type == "string") param->type = ParamType::String;
							else if (type == "double") param->type = ParamType::Double;
							else if (type == "bool") param->type = ParamType::Bool;
							else if (type == "int") param->type = ParamType::Int;
							else if (type == "regex") param->type = ParamType::Regex;

							params.push_back(param);
						}

						return params;
					}
				};

				template<typename T, typename ...Ts>
				struct Route : public BaseRoute
				{
					std::function<T(Ts...)> f;

					Route(std::string route) : BaseRoute(route)
					{

					}

					void Execute(std::shared_ptr<http::component::HttpContext> ctx)
					{
						std::string test;
						static std::string type_name_string = typeid(test).name();

						//auto logger = std::shared_ptr<management::Logger>(new management::Logger("Route"));
						T result = f();

						if (typeid(T).name() == type_name_string) {
							auto res = ctx->Response();
							auto string_response = std::shared_ptr<content::HttpStringContent>(
								new content::HttpStringContent((std::string)result, "application/json"));
							res->SetContent(string_response);
						}
					}
				};

				class Controller
				{
				private:
					static std::mutex routes_mutex;
					static std::map<std::string, std::shared_ptr<BaseRoute>> routes;
					
					std::string base_path;
				public:
					Controller() : base_path("")
					{

					}

					Controller(std::string base_path) : base_path(base_path)
					{

					}

					virtual ~Controller()
					{

					}

					static std::map<std::string, std::shared_ptr<BaseRoute>> Routes()
					{
						return routes;
					}

					template<typename T, typename ...Ts>
					void RegisterRoute(std::string method, std::string route, std::vector<std::string> param_names, 
						typename identity<std::function<T(Ts...)>>::type f)
					{
						auto r = std::shared_ptr<Route<T, Ts...>>(new Route<T, Ts...>(route));
						r->f = f;
						r->method = method;
						r->param_names = param_names;

						std::unique_lock<std::mutex> lk(routes_mutex);
						routes[route] = r;
						lk.unlock();
					}

					template<typename T, typename ...Ts>
					void Get(std::string route, std::vector<std::string> param_names, typename identity<std::function<T(Ts...)>>::type f)
					{
						RegisterRoute<T, Ts...>("GET", route, param_names, f);
					}

					template<typename T, typename ...Ts>
					void Post(std::string route, std::vector<std::string> param_names, typename identity<std::function<T(Ts...)>>::type f)
					{
						RegisterRoute<T, Ts...>("POST", route, param_names, f);
					}

					template<typename T, typename ...Ts>
					void Put(std::string route, std::vector<std::string> param_names, typename identity<std::function<T(Ts...)>>::type f)
					{
						RegisterRoute<T, Ts...>("PUT", route, param_names, f);
					}

					template<typename T, typename ...Ts>
					void Delete(std::string route, std::vector<std::string> param_names, typename identity<std::function<T(Ts...)>>::type f)
					{
						RegisterRoute<T, Ts...>("DELETE", route, param_names, f);
					}
				};
			}
		}
	}
}

#endif
