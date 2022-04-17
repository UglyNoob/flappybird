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
					switch(event.window.event) {
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

	struct RenderBase_T {
	public:
		RenderBase_T(Client *c) : client(*c) {}

		void operator()(bool enable_base_animation = true) {
			constexpr static uint_type PIXELS_PER_SECOND = 60;
			const static UArea base_surface_area = {
				static_cast<uint_type>(Assets::base_surface()->w),
				static_cast<uint_type>(Assets::base_surface()->h),
			};
			if(enable_base_animation) {
				timer = SDL_GetTicks64();
			}
			uint_type start_x = timer * PIXELS_PER_SECOND / 1000 % base_surface_area.w;

			/* the variable x is on the window, while start_x is on the base surface. */
			for(uint_type x = 0; x < client.window_area.w; x += base_surface_area.w) {
				URect srcrect { {start_x, 0}, {base_surface_area.w - start_x, base_surface_area.h} };
				URect dstrect { {x, client.window_area.h - base_surface_area.h}, srcrect.area};
				SDL_RenderCopy(client.renderer, client.assets.base_texture(), srcrect, dstrect);

				srcrect = { {0, 0}, {start_x, base_surface_area.h} };
				dstrect.coord.x = x + base_surface_area.w - start_x;
				dstrect.area = srcrect.area;
				SDL_RenderCopy(client.renderer, client.assets.base_texture(), srcrect, dstrect);
			}
		}
	private:
		decltype(SDL_GetTicks64()) timer = 0;
		Client &client;
	} render_base(this);

	while(true) {
		EventRequests requests = handle_events();
		if(requests.request_exit) break;
		if(requests.window_area_changed)
			window_area = requests.changed_window_area;


		SDL_SetRenderDrawColor(renderer, 0xaa, 0xaa, 0xff, 0xff);
		SDL_RenderFillRect(renderer, nullptr);
		switch (state) {
			case ClientState::MAINMENU:
				render_base();
				break;
			case ClientState::GAMING:
				render_base();
				break;
			case ClientState::PAUSEMENU:
				render_base(false); // static base
				break;
			case ClientState::OVERMENU:
				render_base(false); // static base
				break;
		}

		SDL_RenderPresent(renderer);
	}
}
