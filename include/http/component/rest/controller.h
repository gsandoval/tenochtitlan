#ifndef _REST_CONTROLLER_H_
#define _REST_CONTROLLER_H_

#include "http/component/httpcontext.h"
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <mutex>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			namespace rest
			{
				template <typename T>
				struct identity
				{
					typedef T type;
				};

				struct BaseRoute
				{
					std::string method;
					std::vector<std::string> param_names;
					virtual void Execute(std::shared_ptr<http::component::HttpContext> ctx) = 0;
				};

				template<typename T, typename ...Ts>
				struct Route : public BaseRoute
				{
					std::function<T(Ts...)> f;
					void Execute(std::shared_ptr<http::component::HttpContext> ctx)
					{
						static std::string type_name_string = typeid("").name();
						if (typeid(T).name() == type_name_string) {

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
						auto r = std::make_shared<Route<T, Ts...>>();
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
