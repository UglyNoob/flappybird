#ifndef __FLA_STRUCTURE_H__
#define __FLA_STRUCTURE_H__

#include <cstddef>
#include <type_traits>
#include <cstdio>

#include <SDL2/SDL_rect.h>

using uint_type = size_t;
using int_type = std::make_signed_t<size_t>;

struct Coord {
	int_type x, y;
};

inline constexpr bool operator==(Coord lfs, Coord rfs) { return lfs.x == rfs.x && lfs.y == rfs.y; }
inline constexpr bool operator!=(Coord lfs, Coord rfs) { return !( lfs == rfs ); }

struct UCoord {
	uint_type x, y;
};
inline constexpr bool operator==(UCoord lfs, UCoord rfs) { return lfs.x == rfs.x && lfs.y == rfs.y; }
inline constexpr bool operator!=(UCoord lfs, UCoord rfs) { return !( lfs == rfs ); }

struct UArea {
	uint_type w, h;
};
inline constexpr bool operator==(UArea lfs, UArea rfs) { return lfs.w == rfs.w && lfs.h == rfs.h; }
inline constexpr bool operator!=(UArea lfs, UArea rfs) { return !( lfs == rfs ); }

struct Area {
	int_type w, h;
};
inline constexpr bool operator==(Area lfs, Area rfs) { return lfs.w == rfs.w && lfs.h == rfs.h; }
inline constexpr bool operator!=(Area lfs, Area rfs) { return !( lfs == rfs ); }

struct Rect {
	Coord coord;
	UArea area;

	constexpr operator SDL_Rect() const noexcept {
		SDL_Rect rect{};
		rect.x = coord.x;
		rect.y = coord.y;
		rect.w = area.w;
		rect.h = area.h;
		return rect;
	}
};
inline constexpr bool operator==(Rect lfs, Rect rfs) { return lfs.coord == rfs.coord && lfs.area == rfs.area; }
inline constexpr bool operator!=(Rect lfs, Rect rfs) { return !( lfs == rfs ); }

struct URect {
	UCoord coord;
	UArea area;

	constexpr operator SDL_Rect() const noexcept {
		SDL_Rect rect{};
		rect.x = coord.x;
		rect.y = coord.y;
		rect.w = area.w;
		rect.h = area.h;
		return rect;
	}
};
inline constexpr bool operator==(URect lfs, URect rfs) { return lfs.coord == rfs.coord && lfs.area == rfs.area; }
inline constexpr bool operator!=(URect lfs, URect rfs) { return !( lfs == rfs ); }

void log_normal(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif
