#include "http/httpsocketserverworkercreator.h"

namespace tenochtitlan
{
	namespace http
	{
		using namespace std;

		HttpSocketServerWorkerCreator::HttpSocketServerWorkerCreator(shared_ptr<HttpRequestProcessor> processor)
		{
			this->processor = processor;
		}

		shared_ptr<server::SocketServerWorker> HttpSocketServerWorkerCreator::Create()
		{
			auto worker = make_shared<server::HttpSocketServerWorker>();
			worker->SetRequestProcessor(processor);
			return worker;
		}
	}
}
