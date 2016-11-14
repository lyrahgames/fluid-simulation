#ifndef __VEC_H__
#define __VEC_H__

#include <cstring>
#include <cmath>
// #include <util.h>


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

template <class T, std::size_t N>
inline vec<T,N> operator+(const vec<T,N>& v1, const vec<T,N>& v2){
	vec<T,N> tmp{};
	for (uint i = 0; i < N; i++){
		tmp[i] = v1[i] + v2[i];
	}
	return tmp;
}

template <class T, std::size_t N>
inline vec<T,N> operator*(const vec<T,N>& v1, const vec<T,N>& v2){
	vec<T,N> tmp{};
	for (uint i = 0; i < N; i++){
		tmp[i] = v1[i] * v2[i];
	}
	return tmp;
}

template <class T, std::size_t N>
inline vec<T,N> operator*(T s, const vec<T,N>& v){
	vec<T,N> tmp{};
	for (uint i = 0; i < N; i++){
		tmp[i] = s * v[i];
	}
	return tmp;
}


using vec2 = vec<float,2>;


#endif // __VEC_H__