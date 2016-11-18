#ifndef __INTVL_H__
#define __INTVL_H__


namespace xmath{


template <class T>
struct intvl{
	union{
		struct{T min, max;};
		struct{T inf, sup;};
	};
};


template <class T>
inline constexpr T len(const intvl<T>& i){
	return i.max - i.min;
}


} // xmath


#endif // __INTVL_H__