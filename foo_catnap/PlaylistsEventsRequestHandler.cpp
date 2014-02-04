#include "stdafx.h"

#include "Poco/NumberParser.h"
#include "Poco/Thread.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "PlaylistsEventsRequestHandler.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace CatNap
{
	PlaylistsEventsRequestHandler::PlaylistsEventsRequestHandler()
	{
	}

	void PlaylistsEventsRequestHandler::run()
	{
		if (request().getMethod() == HTTPRequest::HTTP_HEAD || request().getMethod() == HTTPRequest::HTTP_GET)
		{
			if (request().has("Accept") && (request().get("Accept") == "text/event-stream")) {
				unsigned lastEventId = 0;
				if (request().has("Last-Event-ID")) {
					// Process value of Last-Event-ID as base 10 number
					if (!Poco::NumberParser::tryParseUnsigned(request().get("Last-Event-ID"), lastEventId)) {
						lastEventId = 0;
					}
				}

				//response().setChunkedTransferEncoding(true);
				response().setKeepAlive(true);
				response().setContentType("text/event-stream");
				response().set("Cache-Control", "no-cache");

				std::ostream& out = response().send();
				if (request().getMethod() == HTTPRequest::HTTP_GET)
				{
					out << "event:test\r\n";
					out << "id:" << (++lastEventId) << "\r\n";
					out << "data:start\r\n";
					out << "\r\n";
					out << std::flush;
					Poco::Thread::sleep(1000);
					out << "event:test\r\n";
					out << "id:" << (++lastEventId) << "\r\n";
					out << "data:stop\r\n";
					out << "\r\n";
					out << std::flush;
				}
			} else {
				sendErrorResponse(HTTPResponse::HTTP_NO_CONTENT, "");
			}
		}
		else
		{
			sendErrorResponse(HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "");
		}
	}

} // namespace CatNap
