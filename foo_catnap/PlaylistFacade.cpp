#include "stdafx.h"

#include "foobar2000/SDK/foobar2000.h"

#include "PlaylistFacade.h"

namespace CatNap {

	PlaylistFacade::PlaylistFacade()
		: getPlaylistInfos(this, &PlaylistFacade::getPlaylistInfosImpl)
	{
	}

	std::vector<PlaylistInfo::Ptr> PlaylistFacade::getPlaylistInfosImpl(const Poco::Void&)
	{
		static_api_ptr_t<playlist_manager_v4> api;

		const bool main_thread = core_api::is_main_thread();

		const t_size count = api->get_playlist_count();
		const t_size active = api->get_active_playlist();
		const t_size playing = api->get_playing_playlist();

		std::vector<PlaylistInfo::Ptr> infos;
		infos.reserve(count);

		pfc::string8 name;
		for (t_size index = 0; index < count; ++index)
		{
			name.reset();
			if (api->playlist_get_name(index, name))
			{
				infos.push_back(new PlaylistInfo(name.toString(), active == index, playing == index));
			}
		}

		return infos;
	}

} // namespace CatNap
