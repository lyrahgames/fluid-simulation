#ifndef __VEC_H__
#define __VEC_H__

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



	// template <class U, std::size_t M>
	// vec&(const vec<U,M>& src){
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


} // xmath


#endif // __VEC_H__