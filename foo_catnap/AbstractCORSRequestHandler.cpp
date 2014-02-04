#include "AbstractCORSRequestHandler.h"

#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/HTTPResponse.h"

using Poco::Net::MessageHeader;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace CatNap {

AbstractCORSRequestHandler::AbstractCORSRequestHandler() : _pRequest(0), _pResponse(0) {
}

AbstractCORSRequestHandler::~AbstractCORSRequestHandler() {
}

void AbstractCORSRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	_pRequest = &request;
	_pResponse = &response;
	if (authenticate())
	{
		if (request.has("origin") && isAllowedOrigin(request.get("Origin")))
		{
			response.set("Access-Control-Allow-Origin", request.get("Origin"));
			if (request.getMethod() == HTTPRequest::HTTP_OPTIONS)
			{
				response.setContentType("application/json");
				response.setKeepAlive(true);
				try
				{
					if (request.has("Access-Control-Request-Method"))
					{
						const std::string& method = request.get("Access-Control-Request-Method");

						if (isAllowedMethod(method))
						{
							response.set("Access-Control-Allow-Method", method);
						}
					}
					if (request.has("Access-Control-Request-Headers"))
					{
						const std::string& headers = request.get("Access-Control-Request-Headers");
						std::vector<std::string> list;
						MessageHeader::splitElements(headers, list);
						std::string allowHeaders;
						for (auto it = list.begin(); it != list.end(); ++it)
						{
							if (isAllowedHeader(*it))
							{
								if (!allowHeaders.empty())
								{
									allowHeaders += ", ";
								}
								allowHeaders += *it;
							}
						}
						response.set("Access-Control-Allow-Headers", allowHeaders);
					}
					response.send();
				}
				catch (Poco::Exception& exc)
				{
					if (!response.sent())
					{
						sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.displayText());
					}
				}
				catch (std::exception& exc)
				{
					if (!response.sent())
					{
						sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.what());
					}
				}
			}
			else if (isAllowedMethod(request.getMethod()))
			{
				response.setContentType("application/json");
				response.setKeepAlive(true);
				try
				{
					run();
				}
				catch (Poco::Exception& exc)
				{
					if (!response.sent())
					{
						sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.displayText());
					}
				}
				catch (std::exception& exc)
				{
					if (!response.sent())
					{
						sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.what());
					}
				}
			}
			else
			{
				sendErrorResponse(HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "");
			}
		}
		else
		{
			sendErrorResponse(HTTPResponse::HTTP_UNAUTHORIZED, "");
		}
	}
	else
	{
		sendErrorResponse(HTTPResponse::HTTP_UNAUTHORIZED, "");
	}
}

HTTPServerRequest& AbstractCORSRequestHandler::request()
{
	poco_check_ptr(_pRequest);

	return *_pRequest;
}

HTTPServerResponse& AbstractCORSRequestHandler::response()
{
	poco_check_ptr(_pResponse);

	return *_pResponse;
}

void AbstractCORSRequestHandler::sendErrorResponse(HTTPResponse::HTTPStatus status, const std::string& message)
{
	response().setStatusAndReason(status, message);
	response().send();
}

bool AbstractCORSRequestHandler::authenticate()
{
	return true;
}

bool AbstractCORSRequestHandler::isAllowedMethod(const std::string& method)
{
	return true;
}

bool AbstractCORSRequestHandler::isAllowedHeader(const std::string& header)
{
	return true;
}

bool AbstractCORSRequestHandler::isAllowedOrigin(const std::string& origin)
{
	return true;
}

} // namespace // CatNap
