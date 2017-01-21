#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstring>
#include <vector>

#include <xmath/intvl.h>
#include <xmath/linmap.h>
#include <xmath/vec.h>
#include <xmath/field.h>


using uint = unsigned int;
using sint = std::size_t;

using vectorf = std::vector<float>;
using vectoru = std::vector<uint>;

using vecf2 = xmath::vec<float,2>;
using vecu2 = xmath::vec<uint,2>; 
using veci2 = xmath::vec<int,2>;
using intvlf = xmath::intvl<float>;
using intvlv = xmath::intvl<vecf2>;
using linmapf = xmath::linmap<float>;
using linmapv = xmath::linmap<vecf2>;

using fieldf = xmath::field<float>;

using namespace xmath::op;


template <class T, class N>
inline constexpr T lin_interp(T v1, T v2, N w){
	return v1 + w * (v2 - v1);
}

template <class T>
inline T bilin_interp(T v00, T v10, T v01, T v11, T wx, T wy){
	const T tmp1 = lin_interp(v00, v10, wx);
	const T tmp2 = lin_interp(v01, v11, wx);
	return lin_interp(tmp1, tmp2, wy);
}


#endif // __UTIL_H__