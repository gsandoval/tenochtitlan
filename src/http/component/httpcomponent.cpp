#include "http/component/httpcomponent.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			void HttpComponent::SetNext(shared_ptr<HttpComponent> next_component)
			{
				this->next_component = next_component;
			}

			void HttpComponent::Next(shared_ptr<HttpContext> ctx)
			{
				Execute(ctx);
				next_component->Next(ctx);
			}

			void HttpComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				
			}
		}
	}
}
