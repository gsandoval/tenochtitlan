#include "http/component/inputvalidationcomponent.h"
#include "socket/buffer.h"
#include <fstream>
#include <sstream>
#include <cstdio>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			InputValidationComponent::InputValidationComponent()
			{
				logger = shared_ptr<management::Logger>(new management::Logger("InputValidationComponent"));
			}

			void InputValidationComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				auto props = ctx->Properties();
				
				auto req = ctx->Request();
				auto res = ctx->Response();
				res->SetVersion(req->Version());
				res->AddHeader("Server", "tenochtitlan/0.1");

				props->Set("t:IsValid", true);
			}
		}
	}
}
