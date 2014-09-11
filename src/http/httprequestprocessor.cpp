#include "http/httprequestprocessor.h"
#include "http/component/httpcontext.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		void HttpRequestProcessor::ProcessRequest(shared_ptr<component::HttpContext> ctx)
		{
			if (components.size() > 0)
				components[0]->Next(ctx);
		}

		void HttpRequestProcessor::AddComponent(shared_ptr<component::HttpComponent> component)
		{
			shared_ptr<component::HttpComponent> last = nullptr;
			if (components.size() > 0) 
				last = components[components.size() - 1];
			components.push_back(component);
			if (last)
				last->SetNext(component);
		}
	}
}

