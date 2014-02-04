#include "stdafx.h"
#include "CatNapServer.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"

#include <iostream>

#include "CatNapRequestHandlerFactory.h"

using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;

namespace CatNap {

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
