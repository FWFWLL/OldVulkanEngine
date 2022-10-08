#ifndef UTILS_HPP
#define UTILS_HPP

// STD
#include <functional>

namespace FFL {

// From: https://stackoverflow.com/a/57595105
template<typename T, typename... Rest>
void hashCombine(std::size_t& p_seed, const T& p_v, const Rest&... p_rest) {
	p_seed ^= std::hash<T>{}(p_v) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2);
	(hashCombine(p_seed, p_rest), ...);
};

}

#endif
