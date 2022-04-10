#include "assets.h"
#include "basic.h"
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iterator>
#include <array>
#include <memory>
#include <SDL2/SDL.h>


constexpr std::array raw_png_data = []() {
	using char_t = stbi_uc;
	constexpr const char_t origin[] = {
		#include "encoded_assets_png.txt"
	};
	std::array<char_t, std::size(origin) / 2> result{};
	for(size_t i = 0; i < result.size(); ++i) {
		char c = (origin[i * 2] - 'A') << 4;
		c += origin[i * 2 + 1] - 'A';
		result[i] = c;
	}
	return result;
}();
using SDL_Surface_p = std::unique_ptr<SDL_Surface, decltype([] (SDL_Surface *s) { SDL_FreeSurface(s); })>;

bool Assets::surface_initialized = false;

#define load_asset(asset_name, x, y, w, h) SDL_Surface *Assets::asset_name##_surface_ = nullptr; // Define assets surface
#include "assets_cpp_generated_initializing_assets_list.txt"
#undef load_asset

Assets::Assets(Assets &&a) : initialized(a.initialized) {
	if(initialized) {
		#define load_asset(asset_name, x, y, w, h) { \
			asset_name##_texture_ = a.asset_name##_texture_; \
			a.asset_name##_texture_ = nullptr; \
		}
		#include "assets_cpp_generated_initializing_assets_list.txt"
		#undef load_asset
	}
}

void Assets::surface_initialize() {
	assert(!surface_initialized);
	SDL_Surface_p origin_assets_surface;
	int w, h;
	auto data = stbi_load_from_memory(raw_png_data.data(), raw_png_data.size(), &w, &h, nullptr, 4);
	origin_assets_surface.reset(SDL_CreateRGBSurface(0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF));
	SDL_SetSurfaceBlendMode(origin_assets_surface.get(), SDL_BLENDMODE_NONE);
	for(int y = 0; y < h; ++y) {
		for(int x = 0; x < w; ++x) {
			const SDL_Rect pixel_rect{x, y, 1, 1};
			Uint8 r = data[y * w * 4 + x * 4];
			Uint8 g = data[y * w * 4 + x * 4 + 1];
			Uint8 b = data[y * w * 4 + x * 4 + 2];
			Uint8 a = data[y * w * 4 + x * 4 + 3];
			SDL_FillRect(origin_assets_surface.get(), &pixel_rect, SDL_MapRGBA(origin_assets_surface->format, r, g, b, a));
		}
	}
	stbi_image_free(data);

	#define load_asset(asset_name, x, y, w, h) { \
		asset_name##_surface_ = SDL_CreateRGBSurface(0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF); \
		SDL_Rect srcrect = {x, y, w, h}; \
		SDL_Rect dstrect = {0, 0, w, h}; \
		SDL_BlitSurface(origin_assets_surface.get(), &srcrect, asset_name##_surface_, &dstrect); \
	}

	#include "assets_cpp_generated_initializing_assets_list.txt"

	#undef load_asset

	surface_initialized = true;
}

void Assets::surface_free() {
	log_normal("I was called!");
	#define free_asset(asset_name) SDL_FreeSurface(asset_name##_surface_);
	#include "assets_cpp_generated_free_assets_list.txt"
	#undef free_asset
}

bool Assets::initialize(SDL_Renderer *render) {
	assert(!initialized);
	if(!surface_initialized) {
		surface_initialize();
	}

	#define load_asset(asset_name, x, y, w, h) { \
		asset_name##_texture_ = SDL_CreateTextureFromSurface(render, asset_name##_surface_); \
		if(!asset_name##_texture_) { \
			log_error("Error occurred while loading "#asset_name": %s", SDL_GetError()); \
			return false; \
		} \
	}

	#include "assets_cpp_generated_initializing_assets_list.txt"

	#undef load_asset

	initialized = true;

	return true;
}

void Assets::free() {
	assert(initialized);
	#define free_asset(asset_name) { \
		SDL_DestroyTexture(asset_name##_texture_); \
	}
	#include "assets_cpp_generated_free_assets_list.txt"
	#undef free_asset

	initialized = false;
}
