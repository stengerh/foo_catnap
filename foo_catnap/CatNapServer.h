#ifndef CatNap_Server_INCLUDED
#define CatNap_Server_INCLUDED

// Forward declaration, foobar2000 SDK ad Poco::Net header files don't mix well (WSock vs. WSock2 conflict)

namespace Poco {
	namespace Net {
		class HTTPServer;
	}; // namespace Poco
}; // namespace Net

namespace CatNap {

	class CatNapServer {
		Poco::Net::HTTPServer* server;
	public:
		CatNapServer();

		void start();
		void stop();
	};

} // namespace CatNap

#endif // CatNap_Server_INCLUDED
