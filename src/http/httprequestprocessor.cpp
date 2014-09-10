#include "http/httprequestprocessor.h"
#include "http/component/httpcontext.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		shared_ptr<HttpEntity> HttpRequestProcessor::ProcessRequest(shared_ptr<HttpEntity> req)
		{
			auto context = make_shared<component::HttpContext>();
			context->SetRequest(req);
			context->SetResponse(make_shared<HttpEntity>());
			if (components.size() > 0)
				components[0]->Next(context);
			return context->Response();
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

