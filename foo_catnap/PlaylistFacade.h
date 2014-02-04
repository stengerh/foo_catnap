#ifndef CatNap_PlaylistFacade_INCLUDED
#define CatNap_PlaylistFacade_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Void.h"

#include "MainThreadDispatcher.h"
#include "PlaylistInfo.h"

namespace CatNap {

	class PlaylistFacade : public MainThreadDispatcher
	{
	public:
		PlaylistFacade();

		Poco::ActiveMethod<std::vector<PlaylistInfo::Ptr>, Poco::Void, PlaylistFacade, Poco::ActiveStarter<MainThreadDispatcher> > getPlaylistInfos;

	protected:
		std::vector<PlaylistInfo::Ptr> getPlaylistInfosImpl(const Poco::Void&);
	};

} // namespace CatNap

#endif // CatNap_PlaylistFacade_INCLUDED
