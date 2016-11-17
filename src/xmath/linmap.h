#ifndef __LINMAP_H__
#define __LINMAP_H__

#include <xmath/intvl.h>


namespace xmath{


template <class T>
struct linmap{
	using type = T;


	static constexpr T map(const intvl<T>& domain, const intvl<T>& range, const T& x){
		return (len(range)/len(domain)) * (x - domain.min) + range.min;
	}


	constexpr linmap() = default;
	constexpr linmap(const T& slope, const T& intercept):
		slope(slope), intercept(intercept){}
	constexpr linmap(const intvl<T>& domain, const intvl<T>& range):
		slope(len(range)/len(domain)), intercept(range.min - slope * domain.min){}


	constexpr T operator()(const T& x) const{return slope * x + intercept;}


	T slope, intercept;
};

using linmapf = linmap<float>;


template <class T>
inline constexpr linmap<T> inv(const linmap<T>& lm){return linmap<T>{T(1)/lm.slope, -lm.intercept/lm.slope};}


} // xmath


#endif // __LINMAP_H__