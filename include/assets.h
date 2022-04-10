#ifndef __FLAPPYBIRD_RESOURCE_H__
#define __FLAPPYBIRD_RESOURCE_H__

#include <SDL2/SDL.h>
#include <assert.h>


/*
 * Assets
 */

class Assets {

#include "assets_h_generated_assets_list.txt"

public:
	Assets() : initialized(false) {}
	~Assets() { if(initialized) { free(); assert(!initialized); } }
	Assets(const Assets &) = delete;
	Assets(Assets &&);
	Assets &operator=(const Assets &) = delete;

	/*
	 * Initialize textures of assets for specified renderer.
	 * Return false for errors.
	 */
	bool initialize(SDL_Renderer *);
	void free();

	static void surface_initialize();
	static void surface_free();
private:
	bool initialized;

	static bool surface_initialized;
};




#endif
