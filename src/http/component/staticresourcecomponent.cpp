#include "http/component/staticresourcecomponent.h"
#include <fstream>
#include <sstream>

namespace tenochtitlan
{
	namespace http
	{
		namespace component
		{
			using namespace std;

			StaticResourceComponent::StaticResourceComponent() : base_path("")
			{
				logger = shared_ptr<management::Logger>(new management::Logger("StaticResourceComponent"));
			}

			void StaticResourceComponent::Execute(shared_ptr<HttpContext> ctx)
			{
				auto req = ctx->Request();
				if (req->Method() == "GET" && Exists(req->ResourcePath())) {

				}
			}

			bool StaticResourceComponent::Exists(string path)
			{
				ostringstream oss;
				oss << base_path << path;
				ifstream file(path);
				return (bool)file;
			}

			void StaticResourceComponent::SetBasePath(string base_path)
			{
				this->base_path = base_path;
			}
		}
	}
}
