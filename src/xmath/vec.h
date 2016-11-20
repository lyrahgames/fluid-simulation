#ifndef __VEC_H__
#define __VEC_H__

// #include <math.h>
#include <cmath>
#include <algorithm>

#include <xmath/op.h>


namespace xmath{


template <class T, std::size_t N>
struct vec{
	using type = T;
	static constexpr std::size_t _size_ = N;


	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;



	template <class U, std::size_t M>
	vec(const vec<U,M>& src);
	// {
	// 	for (std::size_t i = 0; i < std::min(M,N); i++){
	// 		v[i] = static_cast<T>(src[i]);
	// 	}
	// }

	template <class U, std::size_t M>
	vec& operator=(const vec<U,M>& src){
		for (std::size_t i = 0; i < std::min(M,N); i++){
			v[i] = static_cast<T>(src[i]);
		}
		return *this;
	}

	vec(std::initializer_list<T> l){
		auto it = l.begin();
		for (std::size_t i = 0; it != l.end(); i++, it++)
			v[i] = *it;
	}

	vec(T val){
		for (std::size_t i = 0; i < N; i++)
			v[i] = val;
	}




	static constexpr std::size_t size(){return N;}

	T& operator [](std::size_t idx){return v[idx];}
	constexpr const T& operator [](std::size_t idx) const{return v[idx];}
	

	T v[N];
};

template <>
struct vec<float,2>{
	using type = float;
	static constexpr std::size_t _size_ = 2;


	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;

	template <class U, std::size_t M>
	constexpr vec(const vec<U,M>& src);
	// : x(static_cast<float>(src[0])), y(static_cast<float>(src[1])){}

	template <class U, std::size_t M>
	vec& operator=(const vec<U,M>& src);
	// : x(static_cast<float>(src[0])), y(static_cast<float>(src[1])){return *this;}

	constexpr vec(float val): x(val), y(val){}

	constexpr vec(float a, float b): x(a), y(b){}


	static constexpr std::size_t size(){return 2;}

	float& operator [](std::size_t idx){return v[idx];}
	constexpr const float& operator [](std::size_t idx) const{return v[idx];}
	

	union{
		float v[2];
		struct{float x,y;};
	};
};



template <class T, std::size_t N>
template <class U, std::size_t M>
inline vec<T,N>::vec(const vec<U,M>& src){
	for (std::size_t i = 0; i < std::min(M,N); i++){
		v[i] = static_cast<T>(src[i]);
	}
}

// template <>
template <class U, std::size_t M>
inline constexpr vec<float,2>::vec(const vec<U,M>& src): x(static_cast<float>(src[0])), y(static_cast<float>(src[1])){}

// template <>
template <class U, std::size_t M>
inline vec<float,2>& vec<float,2>::operator=(const vec<U,M>& src){
	x = static_cast<float>(src[0]);
	y = static_cast<float>(src[1]);
	return *this;
}


template <std::size_t M, class Op, class T, std::size_t N>
inline auto pack_op(Op op, const vec<T,N>& v1, const vec<T,N>& v2) -> vec<decltype(op(v1[0], v2[0])),N>{
	vec<decltype(op(v1[0], v2[0])),N> res{};

	for (std::size_t i = 0; i < std::min(N,M); i++)
		res[i] = op(v1[i], v2[i]);

	return res;
}

template <std::size_t M, class Op, class T, std::size_t N>
inline auto pack_op(Op op, const vec<T,N>& v) -> vec<decltype(op(v[0])),N>{
	vec<decltype(op(v[0])),N> res{};

	for (std::size_t i = 0; i < std::min(N,M); i++)
		res[i] = op(v[i]);

	return res;
}




template <class T, std::size_t N>
inline vec<T,N> operator+(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x+y;}, v1, v2);
}

template <std::size_t M, class T, std::size_t N>
inline vec<T,N> add(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<M>([](const T& x, const T& y){return x+y;}, v1, v2);
}

template <class T, std::size_t N>
inline vec<T,N> add(const vec<T,N>& v1, const vec<T,N>& v2){
	return v1 + v2;
}

template <class T, std::size_t N>
inline vec<T,N> operator-(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x-y;}, v1, v2);
}

template <class T, std::size_t N>
inline vec<T,N> operator-(const vec<T,N>& v){
	return pack_op<N>([](const T& x){return -x;}, v);
}

template <class T, std::size_t N>
inline vec<T,N> operator*(const vec<T,N>& v1, const vec<T,N>& v2){
	op::mul mul;
	return pack_op<N>(mul, v1, v2);
}

template <class T, std::size_t N>
inline vec<T,N> operator/(const vec<T,N>& v1, const vec<T,N>& v2){
	op::div div;
	return pack_op<N>(div, v1, v2);
}

template <class T, std::size_t N>
inline vec<T,N> operator*(const T& s, const vec<T,N>& v){
	return pack_op<N>([&s](const T& x){return s*x;}, v);
}

template <class T, std::size_t N>
inline vec<T,N> min(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>(op::min<T>, v1, v2);
}

template <class T, std::size_t N>
inline vec<T,N> sq(const vec<T,N>& v){
	return pack_op<N>(op::sq<T>, v);
}





template <std::size_t N>
inline vec<float,N> fl(const vec<float,N>& v){
	return pack_op<N>(floorf, v);
}

template <std::size_t N>
inline vec<float,N> cl(const vec<float,N>& v){
	return pack_op<N>(ceilf, v);
}


} // xmath


#endif // __VEC_H__