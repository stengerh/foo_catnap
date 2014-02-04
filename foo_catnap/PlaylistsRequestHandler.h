#ifndef CatNap_PlaylistsRequestHandler_INCLUDED
#define CatNap_PlaylistsRequestHandler_INCLUDED

#include "AbstractCORSRequestHandler.h"

namespace CatNap
{

	class PlaylistsRequestHandler : public AbstractCORSRequestHandler
	{
	public:
		PlaylistsRequestHandler();

		void run();
	};

} // namespace CatNap


#endif // CatNap_PlaylistsRequestHandler_INCLUDED
