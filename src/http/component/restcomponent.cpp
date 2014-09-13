#include "http/component/restcomponent.h"

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
				
				logger->Debug(__func__, ctx->Request()->Method());
				logger->Debug(__func__, ctx->Request()->ResourcePath());
				logger->Debug(__func__, ctx->Request()->Version());
				logger->Debug(__func__, ctx->Request()->Host());
			}
		}
	}
}
