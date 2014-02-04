#ifndef CatNap_PlaylistsEventsRequestHandler_INCLUDED
#define CatNap_PlaylistsEventsRequestHandler_INCLUDED

#include "AbstractCORSRequestHandler.h"

namespace CatNap
{
	class PlaylistsEventsRequestHandler : public AbstractCORSRequestHandler
	{
	public:
		PlaylistsEventsRequestHandler();
	
		void run();
	};

} // namespace CatNap

#endif // CatNap_PlaylistsEventsRequestHandler_INCLUDED
