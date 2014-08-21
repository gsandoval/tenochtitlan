#include "management/applicationlifecyclelistener.h"

#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

namespace tenochtitlan
{
	using namespace std;

	ApplicationLifecycleListener _lifecycle_listener;

	void ctrl_c_handler(int s)
	{
		_lifecycle_listener.Shutdown();
	}

	ApplicationLifecycleListener::ApplicationLifecycleListener()
	{
		struct sigaction sig_int_handler;
		sig_int_handler.sa_handler = ctrl_c_handler;
		sigemptyset(&sig_int_handler.sa_mask);
		sig_int_handler.sa_flags = 0;
		sigaction(SIGINT, &sig_int_handler, NULL);
	}

	ApplicationLifecycleListener* ApplicationLifecycleListener::Instance()
	{
		return &_lifecycle_listener;
	}

	void ApplicationLifecycleListener::AddToGlobalPool(shared_ptr<Disposable> disposable)
	{
		global_pool.push_back(disposable);
	}

	void ApplicationLifecycleListener::Shutdown()
	{
		for (unsigned int i = 0; i < global_pool.size(); i++) {
			global_pool[i]->Dispose();
		}
	}
}
