#include "http/component/staticresourcecomponent.h"
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

				ostringstream oss;
				oss << "." << base_path << req->ResourcePath();
				string resource_path = oss.str();

				if (req->Method() == "GET" && Exists(resource_path)) {
					ifstream f(req->ResourcePath());

					ifstream file(resource_path, ios::binary);
					file.seekg(0, ios::end);
					streamsize size = file.tellg();
					file.seekg(0, ios::beg);

					char *file_bytes = new char[size];
					if (!file.read(file_bytes, size)) {
					    delete file_bytes;
					}
					auto buffer = shared_ptr<socket::Buffer>(new socket::Buffer(file_bytes, size));

					res->SetContent(buffer);

					props->Set("t:IsHandled", true);
					props->Set("t:HandledBy", "static_resource_component");

					res->SetCode(200);
					res->SetCodeStr("OK");

					oss = ostringstream();
					oss << buffer->Size();
					res->AddHeader("Content-Type", "text/html");
					res->AddHeader("Content-Length", oss.str());
				}
			}

			bool StaticResourceComponent::Exists(const string &path)
			{
				if (FILE *file = fopen(path.c_str(), "r")) {
					fclose(file);
					return true;
			    } else {
			        return false;
			    }
			    /*
			    struct stat s;
				if( stat(path,&s) == 0 )
				{
				    if( s.st_mode & S_IFDIR )
				    {
				        //it's a directory
				    }
				    else if( s.st_mode & S_IFREG )
				    {
				        //it's a file
				    }
				    else
				    {
				        //something else
				    }
				}
				else
				{
				    //error
				}
			    */
			}

			void StaticResourceComponent::SetBasePath(string base_path)
			{
				this->base_path = base_path;
			}
		}
	}
}
