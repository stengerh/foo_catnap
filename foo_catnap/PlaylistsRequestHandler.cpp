#include "stdafx.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"

#include "PlaylistsRequestHandler.h"
#include "PlaylistFacade.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

using Poco::JSON::Array;
using Poco::JSON::Object;

namespace CatNap
{

	PlaylistsRequestHandler::PlaylistsRequestHandler()
	{
	}

	void PlaylistsRequestHandler::run()
	{
		if (request().getMethod() == HTTPRequest::HTTP_GET || request().getMethod() == HTTPRequest::HTTP_HEAD)
		{
			response().setChunkedTransferEncoding(true);

			std::ostream& out = response().send();
			if (request().getMethod() == HTTPRequest::HTTP_GET)
			{
				PlaylistFacade facade;
				Poco::ActiveResult< std::vector< PlaylistInfo::Ptr > > result = facade.getPlaylistInfos(Poco::Void());
				result.wait();

				std::vector< PlaylistInfo::Ptr > &infos = result.data();

				Object::Ptr obj = new Object();
				Array::Ptr playlists = new Array();
				obj->set("playlists", playlists);
				for (auto it = infos.begin(); it != infos.end(); ++it)
				{
					Object::Ptr playlist = new Object();
					playlists->add(playlist);
					playlist->set("name", (*it)->getName());
					playlist->set("active", (*it)->isActive());
					playlist->set("playing", (*it)->isPlaying());
				}

				obj->stringify(out);
				//out << "{\"playlists\":[{\"name\":\"Default\",\"active\":true,\"playing\":true},{\"name\":\"Search Results\",\"active\":false,\"playlist\":false}]}";
			}
		}
		else
		{
			sendErrorResponse(HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "");
		}
	}

} // namespace CatNap
