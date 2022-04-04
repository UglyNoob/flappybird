#include "resource.h"
#include <iterator>
#include <array>


// constexpr std::array raw_png_data = []() {
// 	constexpr const char origin[] = {
// 		#include "encoded_asserts_png.txt"
// 	};
// 	std::array<char, std::size(origin) / 2> result{};
// 	for(size_t i = 0; i < result.size(); ++i) {
// 		char c = (origin[i * 2] - 'A') << 4;
// 		c += origin[i * 2 + 1] - 'A';
// 		result[i] = c;
// 	}
// 	return result;
// }();


