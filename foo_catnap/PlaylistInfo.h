#ifndef CatNap_PlaylistInfo_INCLUDED
#define CatNap_PlaylistInfo_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"

namespace CatNap {

	class PlaylistInfo : public Poco::RefCountedObject
	{
	public:
		typedef Poco::AutoPtr< PlaylistInfo > Ptr;

		PlaylistInfo(const std::string &name, bool active, bool playing)
			: _name(name), _active(active), _playing(playing)
		{
		}

		inline const std::string &getName() const
		{
			return _name;
		}

		bool isActive() const
		{
			return _active;
		}

		bool isPlaying() const
		{
			return _playing;
		}

	private:
		std::string _name;
		bool _active;
		bool _playing;
	};

} // namespace CatNap

#endif // CatNap_PlaylistInfo_INCLUDED
