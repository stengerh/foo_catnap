#include "stdafx.h"

#include "foobar2000/SDK/foobar2000.h"

#include "MainThreadDispatcher.h"

class main_thread_callback_impl : public main_thread_callback
{
public:
	explicit main_thread_callback_impl(Poco::ActiveRunnableBase::Ptr pRunnable)
		: _pRunnable(pRunnable)
	{
	}

	~main_thread_callback_impl()
	{
	}

	void callback_run()
	{
		_pRunnable->duplicate(); // run will release
		_pRunnable->run();
	}

private:
	Poco::ActiveRunnableBase::Ptr _pRunnable;
};

namespace CatNap {

	MainThreadDispatcher::MainThreadDispatcher()
	{
	}

	void MainThreadDispatcher::start(Poco::ActiveRunnableBase::Ptr pRunnable)
	{
		static_api_ptr_t<main_thread_callback_manager> callback_manager;
		service_ptr_t<main_thread_callback> callback = new service_impl_t<main_thread_callback_impl>(pRunnable);
		callback_manager->add_callback(callback);
	}

} // namespace CatNap
