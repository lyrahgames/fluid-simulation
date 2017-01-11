#ifndef __LINMAP_H__
#define __LINMAP_H__

#include <xmath/intvl.h>


namespace xmath{


template <class T>
struct intvl{
	union{
		struct{T min, max;};
		struct{T inf, sup;};
	};
};

using intvlf = intvl<float>;


template <class T>
inline constexpr T len(const intvl<T>& i){
	return i.max - i.min;
}



template <class T>
struct linmap{
	using type = T;


	static constexpr T map(const intvl<T>& domain, const intvl<T>& range, T input){
		return (range.min * (domain.max - input) + range.max * (input - domain.min)) / (domain.max - domain.min);
	}


	public:
		constexpr linmap(const intvl<T>& domain = {T{0}, T{1}}, const intvl<T>& range = {T{0}, T{1}}):
		d(domain), r(range),
		u{r.max/len(d)}, v{r.min/len(d)},
		s{len(r)/len(d)}, // should be numerical more stable than (v-u)
		i{r.min - s*d.min} // maybe use: (d.max*r.min - d.min*r.max) / len(d)
		{} // compiler should save the inverse of len(d) in a temporary variable


		constexpr T operator()(T input) const{return v * (d.max - input) + u * (input - d.min);}

		constexpr intvl<T> domain() const{return d;}
		constexpr intvl<T> range() const{return r;}
		constexpr T slope() const{return s;}
		constexpr T intercept() const{return i;}
		constexpr T coeff_min() const{return v;}
		constexpr T coeff_max() const{return u;}


	private:
		intvl<T> d; // domain
		intvl<T> r; // range
		T s; // slope
		T i; // intercept
		T u, v; // coefficients for fast interpolation
};

using linmapf = linmap<float>;


template <class T>
inline constexpr linmap<T> inv(const linmap<T>& lm){return linmap<T>(lm.range(), lm.domain());}


} // xmath


#endif // __LINMAP_H__