#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstring>


using uint = unsigned int;
using sint = std::size_t;


template <class T>
inline constexpr T sq(const T& x){
	return x*x;
}

inline constexpr uint quad_rm_memidx(uint in_width, uint in_row, uint in_col){
	return in_width*in_row + in_col;
}


template <class T>
inline T bilin_interp(T v00, T v10, T v01, T v11, T wx, T wy){
	const T wx1 = 1 - wx;
	const T tmp1 = wx1*v00 + wx*v10;
	const T tmp2 = wx1*v01 + wx*v11;
	return (1-wy)*tmp1 + wy*tmp2;
}

template <class T, class N>
inline constexpr T lin_interp(T v1, T v2, N w){
	return (1-w)*v1 + w*v2;
} 


#endif // __UTIL_H__