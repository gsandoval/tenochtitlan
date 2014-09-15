#include "http/component/outputvalidationcomponent.h"
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

			OutputValidationComponent::OutputValidationComponent()
			{
				logger = shared_ptr<management::Logger>(new management::Logger("OutputValidationComponent"));
			}

			void OutputValidationComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				auto props = ctx->Properties();
				auto req = ctx->Request();
				auto res = ctx->Response();
			}
		}
	}
}
