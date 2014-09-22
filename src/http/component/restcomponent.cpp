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
				
				map<string, shared_ptr<::http::component::rest::BaseRoute>> routes = ::http::component::rest::Controller::Routes();
				for (auto it = routes.begin(); it != routes.end(); ++it) {
					if (it->first == resource_path) {
						::http::component::rest::Route* r = it->second.get();
						r->f();
						break;
					}
				}
			}

			void RestComponent::AddController(shared_ptr<::http::component::rest::Controller> ctrl)
			{
				unique_lock<mutex> lk(controllers_mutex);
				controller_list.push_back(ctrl);
				lk.unlock();
			}
		}
	}
}
