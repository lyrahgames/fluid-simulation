#ifndef __OP_H__
#define __OP_H__


namespace xmath{
namespace op{


struct add{
	template <class T>
	constexpr auto operator()(const T& x, const T& y) -> decltype(x+y){
		return x+y;
	}
};
// extern add add;

struct sub{
	template <class T>
	constexpr auto operator()(const T& x, const T& y) -> decltype(x-y){
		return x-y;
	}
};
// extern sub sub;

struct mul{
	template <class T>
	constexpr auto operator()(const T& x, const T& y) -> decltype(x*y){
		return x*y;
	}
};
// extern mul mul;

struct div{
	template <class T>
	constexpr auto operator()(const T& x, const T& y) -> decltype(x/y){
		return x/y;
	}
};
// extern div div;


template <class T>
inline constexpr T min(const T& a, const T& b){
	return (a<b)?(a):(b);
}

template <class T>
inline constexpr T max(const T& a, const T& b){
	return (a>b)?(a):(b);
}

template <class T>
inline constexpr T sq(const T& x){
	return x*x;
}

// template <class T>
// inline constexpr floor(const T& x){
// 	return floor
// }

} // op
} // xmath


#endif // __OP_H__