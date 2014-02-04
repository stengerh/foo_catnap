#include "stdafx.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "RootRequestHandler.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace CatNap
{

	RootRequestHandler::RootRequestHandler()
	{
	}

	void RootRequestHandler::run()
	{
		if (request().getMethod() == HTTPRequest::HTTP_GET || request().getMethod() == HTTPRequest::HTTP_HEAD)
		{
			response().setChunkedTransferEncoding(true);
			response().setContentType("application/json");

			std::ostream& out = response().send();
			if (request().getMethod() == HTTPRequest::HTTP_GET)
			{
				out << "{\"valid\":true}";
			}
		}
		else
		{
			sendErrorResponse(HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "");
		}
	}

} // namespace CatNap
