#pragma once

#include "game/game.hpp"

#include <optional>
#include <string>

namespace discord
{
	game::Material* get_avatar_material(const std::string& id);
	void respond(const std::string& id, int reply);

	// Semantic game state shared by native RPC and launcher IPC; strings are color-stripped and truncated.
	struct presence_state
	{
		bool in_game{false};
		std::string mapname;     // raw map key, empty in menu
		std::string map_display; // friendly map name
		std::string gametype;    // friendly gametype name
		std::string mode;        // short key: "mp" / "sp"
		std::string server_name; // public dedicated server only, else empty
		std::string match_id;    // opaque identity of the match, shared by everyone in it
		int players{0};
		int max_players{0};
	};

	presence_state get_presence_state();

	// Short play-mode key ("mp"/"sp"), valid for the whole process and readable off the main thread.
	std::string get_current_mode();

	// How a friend joins us, in the launcher's unified-secret terms.
	struct join_transport
	{
		bool is_nat{false};
		std::string ip;             // direct: server address
		int port{0};
		std::string token;          // nat: host punch token
		std::string rendezvous_host;
		int rendezvous_port{0};
		std::string fallback_ip;    // nat: host's reachable endpoint (port-forward/VPN/public)
		int fallback_port{0};
	};

	// The current joinable transport, or empty when not joinable (menu / private / unreachable).
	std::optional<join_transport> get_join_transport();

	// Queue a structured join to run once the game is ready; routing an invite mid-load would crash.
	void queue_join(const std::string& token, const std::string& address);

	// Launcher ownership signal from the IPC pipe; native RPC goes silent while the launcher owns presence.
	void set_launcher_presence_owner(bool launcher_owns);
}
