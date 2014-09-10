#include "http/component/staticresourcecomponent.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			StaticResourceComponent::StaticResourceComponent()
			{
				logger = shared_ptr<management::Logger>(new management::Logger("StaticResourceComponent"));
			}

			void StaticResourceComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				logger->Debug(__func__, "");
				logger->Debug(__func__, ctx->Request()->Method());
				logger->Debug(__func__, ctx->Request()->ResourcePath());
				logger->Debug(__func__, ctx->Request()->Version());
				logger->Debug(__func__, ctx->Request()->Host());
			}
		}
	}
}
