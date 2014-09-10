#include "http/httpentity.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		string HttpEntity::Method()
		{
			return method;
		}

		void HttpEntity::SetMethod(string method)
		{
			this->method = method;
		}
		
		string HttpEntity::Version()
		{
			return version;
		}
		
		void HttpEntity::SetVersion(string version)
		{
			this->version = version;
		}
		
		string HttpEntity::Host()
		{
			return host;
		}
		
		void HttpEntity::SetHost(string host)
		{
			this->host = host;
		}
		
		string HttpEntity::ResourcePath()
		{
			return resource_path;
		}
		
		void HttpEntity::SetResourcePath(string resource_path)
		{
			this->resource_path = resource_path;
		}
		
		string HttpEntity::Header(string name)
		{
			return "";
		}
		
		void HttpEntity::AddHeader(string name, string value)
		{
			headers.push_back(make_pair(name, value));
		}
	}
}
