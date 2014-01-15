#include "stdafx.h"
#include "catnap_server.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"

#include <iostream>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerRequest;

namespace CatNap {

class CatNapRequestHandler : public HTTPRequestHandler
{
public:
	CatNapRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		if (request.getMethod() == HTTPServerRequest::HTTP_OPTIONS)
		{
			response.add("Access-Control-Allow-Origin", "*");
			response.add("Access-Control-Allow-Headers", "ContentType");

			response.send();
		}
		else if (request.getMethod() == HTTPServerRequest::HTTP_GET || request.getMethod() == HTTPServerRequest::HTTP_HEAD)
		{
			response.setStatusAndReason(HTTPServerResponse::HTTP_OK);

			response.add("Access-Control-Allow-Origin", "*");

			response.setChunkedTransferEncoding(true);
			response.setContentType("application/json");

			std::ostream& out = response.send();
			if (request.getMethod() == HTTPServerRequest::HTTP_GET)
			{
				out << "{\"valid\":true}";
			}
		}
		else
		{
			response.setStatusAndReason(HTTPServerResponse::HTTP_METHOD_NOT_ALLOWED);
			response.send();
		}
	}
};

class CatNapPlaylistsRequestHandler : public HTTPRequestHandler
{
public:
	CatNapPlaylistsRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		if (request.getMethod() == HTTPServerRequest::HTTP_OPTIONS)
		{
			response.add("Access-Control-Allow-Origin", "*");
			response.add("Access-Control-Allow-Headers", "ContentType");

			response.send();
		}
		else if (request.getMethod() == HTTPServerRequest::HTTP_GET || request.getMethod() == HTTPServerRequest::HTTP_HEAD)
		{
			response.setStatusAndReason(HTTPServerResponse::HTTP_OK);

			response.add("Access-Control-Allow-Origin", "*");

			response.setChunkedTransferEncoding(true);
			response.setContentType("application/json");

			std::ostream& out = response.send();
			if (request.getMethod() == HTTPServerRequest::HTTP_GET)
			{
				out << "{\"playlists\":[{\"name\":\"Default\",\"active\":true,\"playing\":true},{\"name\":\"Search Results\",\"active\":false,\"playlist\":false}]}";
			}
		}
		else
		{
			response.setStatusAndReason(HTTPServerResponse::HTTP_METHOD_NOT_ALLOWED);
			response.send();
		}
	}
};

class CatNapRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	CatNapRequestHandlerFactory()
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/")
			return new CatNapRequestHandler();
		else if (request.getURI() == "/playlists")
			return new CatNapPlaylistsRequestHandler();
		else
			return 0;
	}
};










CatNapServer::CatNapServer() : server(NULL)
{
}

void CatNapServer::start()
{
	unsigned short port = 20402; // 0x4FB2
	HTTPServerParams *params = new HTTPServerParams();

	server = new HTTPServer(new CatNapRequestHandlerFactory(), port, params);
	server->start();
}

void CatNapServer::stop()
{
	if (server != NULL)
	{
		server->stop();
		delete server;
	}
}

} // namespace CatNap
