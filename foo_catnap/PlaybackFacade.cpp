#include "stdafx.h"

#include "foobar2000/SDK/foobar2000.h"

#include "PlaybackFacade.h"

using Poco::AbstractObserver;
using Poco::Notification;
using Poco::NotificationCenter;

class registration_callback : public main_thread_callback
{
public:
	registration_callback(play_callback *p_callback, bool p_enable)
		: m_callback(p_callback), m_enable(p_enable)
	{
	}

	void callback_run()
	{
		static_api_ptr_t<play_callback_manager> callback_manager;
		if (m_enable)
		{
			const unsigned int flags =
				play_callback::flag_on_playback_starting |
				play_callback::flag_on_playback_stop |
				play_callback::flag_on_playback_pause |
				play_callback::flag_on_playback_new_track;
			callback_manager->register_callback(m_callback, flags, true);
		}
		else
		{
			callback_manager->unregister_callback(m_callback);
		}
	}

private:
	play_callback *m_callback;
	bool m_enable;
};

namespace CatNap {

	class PlaybackFacade::play_callback_impl : public play_callback
	{
	public:
		play_callback_impl(NotificationCenter &center, PlaybackNotification::Ptr &pLastNf, Poco::Mutex &mutex):
			_center(center),
			_pLastNf(pLastNf),
			_mutex(mutex),
			_started(false),
			_paused(false),
			_mapValid(false)
		{
			postNotification();
		}

		//! Playback process is being initialized. on_playback_new_track() should be called soon after this when first file is successfully opened for decoding.
		void FB2KAPI on_playback_starting(play_control::t_track_command p_command,bool p_paused)
		{
			_started = true;
			_paused = p_paused;
			postNotification();
		}

		//! Playback advanced to new track.
		void FB2KAPI on_playback_new_track(metadb_handle_ptr p_track)
		{
			_map.clear();
			file_info_impl info;
			if (p_track->get_info(info))
			{
				copy(info, "artist");
				copy(info, "album");
				copy(info, "title");
			}
			_mapValid = true;
			postNotification();
		}

	private:
		void copy(const file_info &info, const char*name)
		{
			const t_size name_index = info.meta_find(name);
			if (name_index != pfc::infinite_size) {
				const t_size value_count = info.meta_enum_value_count(name_index);
				for (t_size value_index = 0; value_index < value_count; value_index++)
				{
					const char *value = info.meta_enum_value(name_index, value_index);
					_map.insert(std::make_pair(name, value));
				}
			}
		}
	public:

		//! Playback stopped.
		void FB2KAPI on_playback_stop(play_control::t_stop_reason p_reason)
		{
			_started = false;
			_paused = false;
			_mapValid = false;
			_map.clear();
			postNotification();
		}

		//! User has seeked to specific time.
		void FB2KAPI on_playback_seek(double p_time)
		{
		}

		//! Called on pause/unpause.
		void FB2KAPI on_playback_pause(bool p_state)
		{
			_paused = p_state;
			postNotification();
		}

		//! Called when currently played file gets edited.
		void FB2KAPI on_playback_edited(metadb_handle_ptr p_track)
		{
		}

		//! Dynamic info (VBR bitrate etc) change.
		void FB2KAPI on_playback_dynamic_info(const file_info & p_info)
		{
		}

		//! Per-track dynamic info (stream track titles etc) change. Happens less often than on_playback_dynamic_info().
		void FB2KAPI on_playback_dynamic_info_track(const file_info & p_info)
		{
		}

		//! Called every second, for time display
		void FB2KAPI on_playback_time(double p_time)
		{
		}

		//! User changed volume settings. Possibly called when not playing.
		//! @param p_new_val new volume level in dB; 0 for full volume.
		void FB2KAPI on_volume_change(float p_new_val)
		{
		}

	private:
		void postNotification()
		{
			postNotification(new PlaybackNotification(_started, _paused, _mapValid, _map));
		}

		void postNotification(const PlaybackNotification::Ptr &pNf)
		{
			{
				Poco::Mutex::ScopedLock lock(_mutex);
				_pLastNf = pNf;
			}
			_center.postNotification(pNf);
		}

		bool _started;
		bool _paused;
		bool _mapValid;
		Poco::ListMap<std::string, std::string> _map;

		NotificationCenter &_center;
		PlaybackNotification::Ptr &_pLastNf;
		Poco::Mutex &_mutex;
	};

	PlaybackFacade::PlaybackFacade()
	{
		_pCallback = new play_callback_impl(_center, _pLastNotification, _mutex);
		static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<registration_callback>(_pCallback, true));
	}

	PlaybackFacade::~PlaybackFacade()
	{
		static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<registration_callback>(_pCallback, false));
		// delete _pCallback;
	}

	void PlaybackFacade::addObserver(const AbstractObserver &observer)
	{
		PlaybackNotification::Ptr pLastNf;
		{
			Poco::Mutex::ScopedLock lock(_mutex);
			pLastNf = _pLastNotification;
		}

		if (pLastNf && observer.accepts(pLastNf))
		{
			observer.notify(pLastNf);
		}

		_center.addObserver(observer);
	}

	void PlaybackFacade::removeObserver(const AbstractObserver &observer)
	{
		_center.removeObserver(observer);
	}



} // namespace CatNap
