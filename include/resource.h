#ifndef __FLAPPYBIRD_RESOURCE_H__
#define __FLAPPYBIRD_RESOURCE_H__

#include <SDL2/SDL.h>
#include <exception>
#include <string>

class Assets {
public:
	Assets() = default;
	~Assets() {free();}
	void initialize();
	void free();

	SDL_Surface *base() const {return base_;}
private:
	SDL_Surface *base_;

	bool ready;
};




#endif
