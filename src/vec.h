#ifndef __VEC_H__
#define __VEC_H__

#include <cstring>
#include <cmath>


template <class T, std::size_t N>
struct vec{
	// template constants
	using coord_type = T;
	static constexpr std::size_t v_size = N;


	// data
	T v[N];


	// interface
	// return count of elements
	static constexpr std::size_t size(){return N;}

	// access operator
	T& operator [](std::size_t idx){return v[idx];}
	// access operator as const call
	constexpr const T& operator [](std::size_t idx) const{return v[idx];}
};


using vec2 = vec<float,2>;


#endif // __VEC_H__