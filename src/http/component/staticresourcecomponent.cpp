#include "http/component/staticresourcecomponent.h"
#include "socket/buffer.h"
#include "http/content/httpfilecontent.h"
#include "util/filereadercache.h"
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
				auto props = ctx->Properties();
				if (!props->GetBool("t:IsValid") || props->GetBool("t:IsHandled"))
					return;

				auto req = ctx->Request();
				auto res = ctx->Response();

				string rp = req->ResourcePath();
				auto qpos = rp.find_last_of("?");
				if (qpos != string::npos) {
					rp = rp.substr(0, qpos);
				}

				ostringstream oss;
				oss << "." << base_path << rp;
				string resource_path = oss.str();

				logger->Debug(__func__, "Before file exist check");

				auto file_cache = util::FileReaderCache::Instance();
				logger->Debug(__func__, "File cache instance obtained");

				if (req->Method() == "GET" && file_cache->Exists(resource_path)) {
					logger->Debug(__func__, "it does exist");
					auto file_content = file_cache->GetFile(resource_path);
					oss = ostringstream();
					oss << "file size to queue " << file_content->Buffer()->Size();
					logger->Debug(__func__, oss.str());
					res->SetContent(file_content);

					props->Set("t:IsHandled", true);
					props->Set("t:HandledBy", "static_resource_component");

					res->SetCode(200);
					res->SetCodeStr("OK");
				}
			}

			void StaticResourceComponent::SetBasePath(string base_path)
			{
				this->base_path = base_path;
			}
		}
	}
}
