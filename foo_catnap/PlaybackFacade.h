#ifndef CatNap_PlaybackFacade_INCLUDED
#define CatNap_PlaybackFacade_INCLUDED

#include "Poco/ActiveMethod.h"
#include "Poco/AbstractObserver.h"
#include "Poco/Notification.h"
#include "Poco/NotificationCenter.h"
#include "Poco/ListMap.h"
#include "MainThreadDispatcher.h"

namespace CatNap {

	class PlaybackNotification : public Poco::Notification
	{
	public:
		typedef Poco::AutoPtr<PlaybackNotification> Ptr;

		PlaybackNotification(bool started, bool paused, bool mapValid, const Poco::ListMap<std::string, std::string> &map):
			_started(started),
			_paused(paused),
			_mapValid(mapValid),
			_map(map)
		{
		}

		inline bool isStarted() const
		{
			return _started;
		}

		inline bool isPaused() const
		{
			return _paused;
		}

		inline bool isMapValid() const
		{
			return _mapValid;
		}

		inline const Poco::ListMap<std::string, std::string> map() const
		{
			return _map;
		}

	private:
		bool _started;
		bool _paused;
		bool _mapValid;
		Poco::ListMap<std::string, std::string> _map;
	};

	class PlaybackFacade : public MainThreadDispatcher
	{
	public:
		PlaybackFacade();
		~PlaybackFacade();

		void addObserver(const Poco::AbstractObserver &poserver);
		void removeObserver(const Poco::AbstractObserver &observer);

	protected:

	private:
		PlaybackFacade(const PlaybackFacade &);
		class play_callback_impl;

		Poco::NotificationCenter _center;
		play_callback_impl *_pCallback;
		PlaybackNotification::Ptr _pLastNotification;
		Poco::Mutex _mutex;
	};
}

#endif CatNap_PlaybackFacade_INCLUDED
