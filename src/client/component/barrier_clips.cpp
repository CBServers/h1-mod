#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "barrier_clips.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace barrier_clips
{
	namespace
	{
		constexpr int CONTENTS_PLAYERCLIP = 0x10000;
		constexpr int CONTENTS_BARRIERCLIP = 0x400;

		constexpr auto MAX_CLIENTS = 18;

		game::dvar_t* bg_disable_barrier_clips = nullptr;
		game::dvar_t* bg_disable_barrier_clips_client = nullptr;

		// authoritative per-client preference, kept in sync server-side (see set_client_pref)
		bool client_pref[MAX_CLIENTS] = {};

		utils::hook::detour pmove_single_hook;

		bool enabled(const game::mp::playerState_s* ps)
		{
			if (bg_disable_barrier_clips && bg_disable_barrier_clips->current.enabled)
			{
				return true; // server master switch forces the barrier clips off for everyone
			}

			static game::dvar_t* sv_running = nullptr;
			if (!sv_running)
			{
				sv_running = game::Dvar_FindVar("sv_running");
			}

			if (sv_running && sv_running->current.enabled)
			{
				// running the authoritative sim: honour this client's own preference
				const auto client_num = static_cast<unsigned char>(ps->clientNum);
				return client_num < MAX_CLIENTS && client_pref[client_num];
			}

			// remote client: only ever predicts the local player, so use our own local preference
			return bg_disable_barrier_clips_client && bg_disable_barrier_clips_client->current.enabled;
		}

		void pmove_single_stub(game::mp::pmove_t* pm)
		{
			if (pm && pm->ps && enabled(pm->ps) && (pm->ps->pm_flags & game::PMF_LADDER) == 0)
			{
				pm->tracemask &= ~CONTENTS_PLAYERCLIP;
				pm->tracemask |= CONTENTS_BARRIERCLIP;
			}

			pmove_single_hook.invoke<void>(pm);
		}
	}

	void set_client_pref(const int client_num, const bool value)
	{
		if (client_num >= 0 && client_num < MAX_CLIENTS)
		{
			client_pref[client_num] = value;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_sp())
			{
				return;
			}

			pmove_single_hook.create(0x2D19A0_b, pmove_single_stub);

			bg_disable_barrier_clips = dvars::register_bool("bg_disableBarrierClips", false,
				game::DVAR_FLAG_REPLICATED, "Disable player collision with out of bound barriers");

			// per-client opt-in; pushed by the server via `self setclientdvar("bg_disableBarrierClipsClient", 1)`
			// no flags so the client's setclientdvar handler (patches.cpp) accepts it
			bg_disable_barrier_clips_client = dvars::register_bool("bg_disableBarrierClipsClient", false,
				game::DVAR_FLAG_NONE, "Disable player collision with out of bound barriers for this client");
		}
	};
}

REGISTER_COMPONENT(barrier_clips::component)
