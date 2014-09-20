#include "http/httpentity.h"
#include <sstream>

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

		vector<string> HttpEntity::HeaderNames()
		{
			vector<string> header_names;
			for (unsigned int i = 0; i < headers.size(); i++) {
				header_names.push_back(headers[i].first);
			}
			return header_names;
		}
		
		string HttpEntity::Header(string name)
		{
			string value = "";
			for (unsigned int i = 0; i < headers.size(); i++) {
				if (headers[i].first == name)
					value = headers[i].second;
			}
			return value;
		}
		
		void HttpEntity::AddHeader(string name, string value)
		{
			if (name == "Host")
				host = value;

			headers.push_back(make_pair(name, value));
		}

		void HttpEntity::SetContent(string str)
		{
			int size = str.size();
			char *buf = new char[size];
			memcpy(buf, str.c_str(), size);
			content = shared_ptr<socket::Buffer>(new socket::Buffer(buf, size));
		}

		void HttpEntity::SetContent(shared_ptr<socket::Buffer> buffer)
		{
			content = buffer;
		}

		void HttpEntity::SetContent(shared_ptr<content::HttpContent> content)
		{
			this->content = content->Buffer();
			ostringstream oss;
			oss << content->Buffer()->Size();
			AddHeader("Content-Type", content->Mime());
			AddHeader("Content-Length", oss.str());
		}

		shared_ptr<socket::Buffer> HttpEntity::ContentAsBuffer()
		{
			return content;
		}

		string HttpEntity::ContentAsString()
		{
			int size = content->Size();
			char *buf = new char[size + 1];
			memcpy(buf, content->Buf(), size);
			buf[size] = 0;
			string t = string(buf);
			delete buf;
			return t;
		}

		shared_ptr<socket::Buffer> HttpEntity::HeaderAsBuffer()
		{
			ostringstream oss;
			
			oss << version << " " << code << " " << code_str << "\r\n";

			for (unsigned int i = 0; i < headers.size(); i++) {
				oss << headers[i].first << ": " << headers[i].second << "\r\n";
			}

			oss << "\r\n";

			return shared_ptr<socket::Buffer>(new socket::Buffer(oss.str()));
		}

		int HttpEntity::Code()
		{
			return code;
		}

		string HttpEntity::CodeStr()
		{
			return code_str;
		}

		void HttpEntity::SetCode(int code)
		{
			this->code = code;
		}

		void HttpEntity::SetCodeStr(string code_str)
		{
			this->code_str = code_str;
		}
	}
}
