#ifndef CatNap_RequestHandlerFactory_INCLUDED
#define CatNap_RequestHandlerFactory_INCLUDED

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "PlaybackFacade.h"

namespace CatNap {

	class CatNapRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		CatNapRequestHandlerFactory();

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

	private:
		PlaybackFacade playbackFacade;
	};

} // namespace CatNap


#endif // CatNap_RequestHandlerFactory_INCLUDED
