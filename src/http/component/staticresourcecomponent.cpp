#include "http/component/staticresourcecomponent.h"
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
				auto req = ctx->Request();

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
					if (file.read(file_bytes, size)) {
					    logger->Debug(__func__, string(file_bytes));
					}

					delete file_bytes;
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
