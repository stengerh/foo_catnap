#ifndef CatNap_PlaybackEventsRequestHandler_INCLUDED
#define CatNap_PlaybackEventsRequestHandler_INCLUDED

#include "AbstractCORSRequestHandler.h"
#include "PlaybackFacade.h"

namespace CatNap {

	class PlaybackEventsRequestHandler : public AbstractCORSRequestHandler
	{
	public:
		PlaybackEventsRequestHandler(PlaybackFacade &facade);

		void run();

	protected:
		void sendPlaybackEvent(std::ostream &out, const PlaybackNotification &nf);

	private:
		PlaybackFacade &_facade;
	};

}

#endif CatNap_PlaybackEventsRequestHandler_INCLUDED
