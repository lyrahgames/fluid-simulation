#ifndef __TERM_OUT_H__
#define __TERM_OUT_H__

#include <iostream>

#include <xmath/intvl.h>
#include <xmath/linmap.h>
#include <xmath/vec.h>
#include <xmath/field.h>


template <class T>
std::ostream& operator<<(std::ostream& os, const xmath::intvl<T>& i){
	return os << "[" << i.min << ", " << i.max << "]";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const xmath::linmap<T>& lm){
	return os 
		// << lm.domain() << " --> " << lm.range() << "\n"
		<< "slope:\t" << lm.slope << "\tintercept:\t" << lm.intercept;
		// << "(u,v):\t" << lm.coeff_min() << "\t" << lm.coeff_max();
}

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const xmath::vec<T,N>& v){
	os << "(" << v[0];
	for (std::size_t i = 1; i < N; i++)
		os << ", " << v[i];
	return os << ")";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const xmath::field<T>& f){
	os << "size: " << f.size() << "\tdim: " << f.dim() << "\n";
	for (std::size_t j = 0; j < f.dim_y(); j++){
		for (std::size_t i = 0; i < f.dim_x(); i++){
			os << f(i,j) << "\t";
		}
		os << "\n";
	}

	return os;
}


#endif // __TERM_OUT_H__