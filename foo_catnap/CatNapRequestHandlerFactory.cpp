#include "stdafx.h"

#include "CatNapRequestHandlerFactory.h"

#include "RootRequestHandler.h"
#include "PlaybackEventsRequestHandler.h"
#include "PlaylistsRequestHandler.h"
#include "PlaylistsEventsRequestHandler.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace CatNap {

	CatNapRequestHandlerFactory::CatNapRequestHandlerFactory()
	{
	}

	HTTPRequestHandler* CatNapRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/")
			return new RootRequestHandler();
		else if (request.getURI() == "/playback/events")
			return new PlaybackEventsRequestHandler(playbackFacade);
		else if (request.getURI() == "/playlists")
			return new PlaylistsRequestHandler();
		else if (request.getURI() == "/playlists/events")
			return new PlaylistsEventsRequestHandler();
		else
			return 0;
	}

} // namespace CatNap
