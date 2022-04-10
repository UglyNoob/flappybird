#include "client.h"
#include "basic.h"

#include <assert.h>
#include <SDL2/SDL.h>
#include <iterator>

bool SDL2_initialized = false;

uint_type instances_of_client = 0;

bool initialize_SDL2() {
	assert(!SDL2_initialized);
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		log_error("Error occurred while initializing SDL2: %s\n", SDL_GetError());
		return false;
	}

	SDL2_initialized = true;
	return true;
}

bool Client::initialize(ClientSetting setting) {
	assert(!initialized);
	if(!SDL2_initialized) {
		assert(instances_of_client == 0);
		if(!initialize_SDL2()) return false;
	}

	window_area = setting.window_area;
	window = SDL_CreateWindow("Flappybird",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			window_area.w, window_area.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(!window) {
		log_error("Error occurred while creating window: %s", SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		log_error("Error occurred while creating renderer: %s", SDL_GetError());
		return false;
	}
	assets.initialize(renderer);

	++instances_of_client;
	initialized = true;
	state = ClientState::MAINMENU;

	return true;
}

void Client::quit() {
	assert(initialized);

	assets.free();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	--instances_of_client;
	if(instances_of_client == 0) {
		Assets::surface_free();
		SDL_Quit();
		SDL2_initialized = false;
	}
}

Client::Client(Client &&c) : initialized(c.initialized), window(c.window), renderer(c.renderer) {}

struct EventRequests {
	bool request_exit = false;

	bool window_area_changed = false;
	UArea changed_window_area;
};
SDL_Color next_color() {
	return {255, 0, 0, 255};
}

SDL_Color next_color(SDL_Color prv) {
	if(prv.r == 255) {
		if(prv.b == 0) {
			if(prv.g == 255) {
				--prv.r;
			} else {
				++prv.g;
			}
		} else {
			--prv.b;
		}
	} else if(prv.g == 255) {
		if(prv.r == 0) {
			if(prv.b == 255) {
				--prv.g;
			} else {
				++prv.b;
			}
		} else {
			--prv.r;
		}
	} else { // prv.b == 255
		if(prv.r == 0) {
			if(prv.g == 0) {
				++prv.r;
			} else {
				--prv.g;
			}
		} else {
			++prv.r;
		}
	}
	return prv;
}

void Client::main_loop() {
	log_normal("main loop");
	log_warning("main loop");
	log_error("main loop");

	auto handle_events = []() {
		EventRequests result;
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					result.request_exit = true;
					break;
				case SDL_WINDOWEVENT:
					switch(event.window.type) {
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							result.window_area_changed = true;
							result.changed_window_area.w = event.window.data1;
							result.changed_window_area.h = event.window.data2;
							break;
					}
					break;
			}
		}
		return result;
	};

	SDL_Surface *surfaces[] = {
		#define free_asset(asset_name) Assets::asset_name##_surface(),
		#include "assets_cpp_generated_free_assets_list.txt"
		#undef free_asset
	};
	SDL_Texture *textures[] = {
		#define free_asset(asset_name) assets.asset_name##_texture(),
		#include "assets_cpp_generated_free_assets_list.txt"
		#undef free_asset
	};
	uint_type surface_index = 0;
	auto time = SDL_GetTicks64();
	SDL_Color color = next_color();

	while(true) {
		EventRequests requests = handle_events();
		if(requests.request_exit) break;
		if(requests.window_area_changed) window_area = requests.changed_window_area;

		auto time2 = SDL_GetTicks64();
		if(time2 - time > 1000) {
			time = time2;
			++surface_index;
			if(surface_index == std::size(surfaces)) surface_index = 0;
		}

		SDL_Surface *surface = surfaces[surface_index];
		SDL_Texture *texture = textures[surface_index];
		SDL_Rect rect{0, 0, surface->w, surface->h};

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, nullptr);
		SDL_RenderCopy(renderer, texture, &rect, &rect);
		SDL_RenderPresent(renderer);

		color = next_color(color);
	}
}
