#ifndef CatNap_AbstractCORSRequestHandler_INCLUDED
#define CatNap_AbstractCORSRequestHandler_INCLUDED

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

namespace CatNap {

	class AbstractCORSRequestHandler : public Poco::Net::HTTPRequestHandler {
	public:
		AbstractCORSRequestHandler();

		virtual ~AbstractCORSRequestHandler();

		virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	
		Poco::Net::HTTPServerRequest& request();

		Poco::Net::HTTPServerResponse& response();

		void sendErrorResponse(Poco::Net::HTTPResponse::HTTPStatus status, const std::string& message);

	protected:
		virtual void run() = 0;

		virtual bool authenticate();

		virtual bool isAllowedOrigin(const std::string& origin);

		virtual bool isAllowedMethod(const std::string& method);

		virtual bool isAllowedHeader(const std::string& method);

	private:
		Poco::Net::HTTPServerRequest*  _pRequest;
		Poco::Net::HTTPServerResponse* _pResponse;
	};

} // namespace CatNap

#endif // CatNap_AbstractCORSRequestHandler_INCLUDED
