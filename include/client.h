#ifndef __FLAPPYBIRD_CLIENT_H__
#define __FLAPPYBIRD_CLIENT_H__

#include <SDL2/SDL.h>

#include "assets.h"
#include "basic.h"

constexpr UArea CLIENT_DEFAULT_WINDOW_AREA = { 640, 480 };

struct ClientSetting {
	UArea window_area = CLIENT_DEFAULT_WINDOW_AREA;

	bool disable_mainmenu = false;
};

enum class ClientState : uint8_t {
	MAINMENU, GAMING, PAUSEMENU, OVERMENU
};


class Client {
public:
	Client() : initialized(false), window(nullptr), renderer(nullptr) {}

	Client(const Client &) = delete;

	Client(Client &&);

	~Client() {if(initialized) { quit(); } }

	Client &operator=(const Client &) = delete;

	bool initialize(ClientSetting);

	/*
	 * Blocks.
	 * Returns when the window exits.
	 */
	void main_loop();

	/*
	 * Release resources.
	 */
	void quit();
private:
	bool initialized;
	ClientState state;
	UArea window_area;

	SDL_Window *window;
	SDL_Renderer *renderer;
	Assets assets;
};

#endif
