#include "http/content/httpfilecontent.h"

namespace tenochtitlan
{
	namespace http
	{
		namespace content
		{
			using namespace std;

			HttpFileContent::HttpFileContent()
			{
				
			}

			HttpFileContent::HttpFileContent(shared_ptr<socket::Buffer> buffer) : HttpContent(buffer)
			{

			}

			void HttpFileContent::SetMimeForExt(string ext)
			{
				mime = MIME_TYPES[ext];
			}
		}
	}
}