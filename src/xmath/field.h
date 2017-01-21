#ifndef __FIELD_H__
#define __FIELD_H__

#include <string.h>

#include <xmath/vec.h>


namespace xmath{


template <class T>
struct field{
	using type = T;
	using uint = unsigned int;
	using vecu2 = vec<uint,2>;


	public:
		field(): v(nullptr), d(vecu2{}), s(0) {}
		field(const vecu2& dimension): d(dimension), s(d[0]*d[1]), v(nullptr){
			init();
		}
		field(uint width, uint height): d(vecu2{width, height}), s(width*height), v(nullptr){
			init();
		}

		field(const field& f): d(f.d), s(f.s), v(nullptr){
			init();
			memcpy(v, f.v, s * sizeof(T));
		}

		field& operator=(const field& f){
			if (v != nullptr)
				delete[] v;

			d = f.d;
			s = f.s;
			v = new T[s];
			memcpy(v, f.v, s * sizeof(T));

			return *this;
		}

		field(field&& f): d(f.d), v(f.v){
			f.v = nullptr;
		}

		field& operator=(field&& f){
			d = f.d;
			s = f.s;

			// swap pointers
			T*const tmp = f.v;
			f.v = v;
			v = tmp;

			return *this;
		}

		~field(){
			if (v != nullptr)
				delete[] v;
		}


		vecu2 dim() const{
			return d;
		}

		uint dim_x() const{
			return d[0];
		}

		uint dim_y() const{
			return d[1];
		}

		uint size() const{
			return s;
		}

		T& operator[](uint idx){
			return v[idx];
		}

		const T& operator[](uint idx) const{
			return v[idx];
		}

		T& operator[](const vecu2& idx){
			return v[memidx(idx)];
		}

		const T& operator[](const vecu2& idx) const{
			return v[memidx(idx)];
		}

		T& operator()(const vecu2& idx){
			return v[memidx(idx)];
		}

		const T& operator()(const vecu2& idx) const{
			return v[memidx(idx)];
		}

		T& operator()(uint i, uint j){
			return v[memidx(i,j)];
		}

		const T& operator()(uint i, uint j) const{
			return v[memidx(i,j)];
		}

		T* data(){
			return v;
		}

		void setzero(){
			memset(v, 0, s*sizeof(T));
		}


	private:
		T* v; // vector with elements
		vecu2 d; // dimension
		uint s; // size


	public:
		uint memidx(uint i, uint j) const{
			return j * d[0] + i;
		}

		uint memidx(const vecu2& idx) const{
			return idx[1] * d[0] + idx[0];
		}

		template<class T>
		friend void swap_data(field<T>& f, field<T>& g);
		template<class T>
		friend void swap(field<T>& f, field<T>& g);


	private:
		void init(){
			v = new T[s];
		}
};



template<class T>
void swap_data(field<T>& f, field<T>& g){
	T*const tmp = g.v;
	g.v = f.v;
	f.v = tmp;
}

template<class T>
void swap(field<T>& f, field<T>& g){
	const uint tmp1 = g.s;
	g.s = f.s;
	f.s = tmp1;

	const vecu2 tmp2 = g.d;
	g.d = f.d;
	f.d = tmp2;

	swap_data(f,g);
}


} // xmath


#endif // __FIELD_H__