#include "http/content/httpstringcontent.h"
#include <iostream>

namespace tenochtitlan
{
	namespace http
	{
		namespace content
		{
			using namespace std;

			HttpStringContent::HttpStringContent()
			{

			}

			HttpStringContent::HttpStringContent(shared_ptr<socket::Buffer> buffer) : HttpContent(buffer)
			{

			}

			HttpStringContent::HttpStringContent(string content) : HttpStringContent(shared_ptr<socket::Buffer>(new socket::Buffer(content)))
			{
				
			}

			HttpStringContent::HttpStringContent(shared_ptr<socket::Buffer> buffer, string mime) : HttpContent(buffer)
			{
				this->mime = mime;
			}

			HttpStringContent::HttpStringContent(string content, string mime) : 
				HttpStringContent(shared_ptr<socket::Buffer>(new socket::Buffer(content)))
			{
				this->mime = mime;
			}

		}
	}
}
