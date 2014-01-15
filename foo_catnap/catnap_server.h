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
