#ifndef CatNap_RootRequestHandler_INCLUDED
#define CatNap_RootRequestHandler_INCLUDED

#include "AbstractCORSRequestHandler.h"

namespace CatNap
{

	class RootRequestHandler : public AbstractCORSRequestHandler
	{
	public:
		RootRequestHandler();

		void run();
	};

} // namespace CatNap

#endif // CatNap_RootRequestHandler_INCLUDED
