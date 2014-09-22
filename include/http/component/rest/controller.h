#ifndef _REST_CONTROLLER_H_
#define _REST_CONTROLLER_H_

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <mutex>

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
				
			};

			template<typename T, typename ...Ts>
			struct Route : public BaseRoute
			{
				std::function<T(Ts...)> f;
				std::string method;
				std::vector<std::string> param_names;
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

#endif
