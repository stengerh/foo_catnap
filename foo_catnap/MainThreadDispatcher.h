#ifndef CatNap_MainThreadDispatcher_INCLUDED
#define CatNap_MainThreadDispatcher_INCLUDED

#include "Poco/Runnable.h"
#include "Poco/ActiveStarter.h"
#include "Poco/ActiveRunnable.h"

namespace CatNap {
	class MainThreadDispatcher
	{
	public:
		MainThreadDispatcher();

		void start(Poco::ActiveRunnableBase::Ptr pRunnable);
	};

} // namespace CatNap

namespace Poco {

	template<>
	/// A specialization of ActiveStarter
	/// for MainThreadDispatcher.
	class ActiveStarter<CatNap::MainThreadDispatcher>
	{
	public:
		static void start(CatNap::MainThreadDispatcher* pOwner, ActiveRunnableBase::Ptr pRunnable)
		{
			pOwner->start(pRunnable);
		}
	};

} // namespace Poco

#endif // CatNap_MainThreadDispatcher_INCLUDED
