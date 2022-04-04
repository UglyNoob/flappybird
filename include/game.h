#ifndef __FLA_GAME_H__
#define __FLA_GAME_H__

#include "basic_types.h"
#include <cstdint>

enum class GameState : uint8_t {
	MAINMENU, GAMING, PAUSEMENU, OVERMENU
};

struct GameSetting {
	uint_type map_height;
	uint_type jump_velocity;
	uint_type gravity; // down positive
	uint_type maximum_velocity; // both up and down
	uint_type horizontal_speed; // right positive
	uint_type block_width;
	uint_type minimum_block_horizental_interval;
	uint_type maximum_block_horizental_interval;
	uint_type minimum_block_vertical_interval;
	uint_type maximum_block_vertical_interval;
	Size bird_size;

	bool check() {
		return minimum_block_horizental_interval < maximum_block_horizental_interval &&
			minimum_block_vertical_interval < maximum_block_vertical_interval &&
			horizontal_speed < block_width &&
			jump_velocity < maximum_velocity;
	}
};

struct GameData {
	GameState state;
	int_type bird_y;
	int_type bird_velocity; // up positive
};




#endif