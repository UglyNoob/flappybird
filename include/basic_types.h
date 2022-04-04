#ifndef __FLA_STRUCTURE_H__
#define __FLA_STRUCTURE_H__

#include <cstddef>
#include <type_traits>

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

struct USize {
	uint_type w, h;
};
inline constexpr bool operator==(USize lfs, USize rfs) { return lfs.w == rfs.w && lfs.h == rfs.h; }
inline constexpr bool operator!=(USize lfs, USize rfs) { return !( lfs == rfs ); }

struct Size {
	int_type w, h;
};
inline constexpr bool operator==(Size lfs, Size rfs) { return lfs.w == rfs.w && lfs.h == rfs.h; }
inline constexpr bool operator!=(Size lfs, Size rfs) { return !( lfs == rfs ); }

struct Rect {
	Coord coord;
	USize size;
};
inline constexpr bool operator==(Rect lfs, Rect rfs) { return lfs.coord == rfs.coord && lfs.size == rfs.size; }
inline constexpr bool operator!=(Rect lfs, Rect rfs) { return !( lfs == rfs ); }

struct URect {
	UCoord coord;
	USize size;
};
inline constexpr bool operator==(URect lfs, URect rfs) { return lfs.coord == rfs.coord && lfs.size == rfs.size; }
inline constexpr bool operator!=(URect lfs, URect rfs) { return !( lfs == rfs ); }

#endif
