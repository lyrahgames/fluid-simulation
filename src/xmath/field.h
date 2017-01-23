#ifndef __FIELD_H__
#define __FIELD_H__

#include <string.h>
// #include <vector>

#include <xmath/vec.h>


namespace xmath{


template <class T>
struct field{
	using type = T;
	using uint = unsigned int;
	using vecu2 = vec<uint,2>;
	using vect2 = vec<T,2>;
	using intvlv = intvl<vect2>;
	using linmapv = linmap<vect2>;



	public:
		field(const vecu2& dimension, const intvlv& domain):
			v(nullptr), d(dimension), s(d[0]*d[1]), dd(domain),
			gtod({vect2(), {static_cast<T>(dimension[0]-1), static_cast<T>(dimension[1]-1)}}, domain),
			dtog(inv(gtod)){
			init_data();
		}

		field(uint width, uint height, const intvlv& domain):
			field({width,height}, domain){}

		field(const field& f):
			v(nullptr), d(f.d), s(f.s), dd(f.dd), gtod(f.gtod), dtog(f.dtog){
			init_data();
			memcpy(v, f.v, s * sizeof(T));
		}

		field(field&& f): 
			v(f.v), d(f.d), s(f.s), dd(f.dd), gtod(f.gtod), dtog(f.dtog){
			f.v = nullptr;
		}

		~field(){
			free_data();
		}

		field& operator=(const field& f){
			copy_head(f);
			free_data();
			init_data();
			copy_data(f);
			return *this;
		}

		field& operator=(field&& f){
			copy_head(f);
			swap_data(f);
			return *this;
		}



		T* data(){return v;}
		vecu2 dim() const{return d;}
		uint dim_x() const{return d[0];}
		uint dim_y() const{return d[1];}
		uint size() const{return s;}
		const intvlv& domain() const{return dd;}
		const linmapv& grid_to_domain() const{return gtod;}
		const linmapv& domain_to_grid() const{return dtog;}

		T& operator[](uint idx){return v[idx];}
		const T& operator[](uint idx) const{return v[idx];}
		T& operator[](const vecu2& idx){return v[memidx(idx)];}
		const T& operator[](const vecu2& idx) const{return v[memidx(idx)];}
		T& operator()(const vecu2& idx){return v[memidx(idx)];}
		const T& operator()(const vecu2& idx) const{return v[memidx(idx)];}
		T& operator()(uint i, uint j){return v[memidx(i,j)];}
		const T& operator()(uint i, uint j) const{return v[memidx(i,j)];}

		T operator()(const vec<T,2>& pos) const{
			const vect2 midx = dtog(pos);
			const vect2 sidx = fl(midx);
			const vecu2 idx = sidx;
			const vect2 w = midx - sidx;

			const uint idx1 = memidx(idx[0], idx[1]);
			// const uint idx2 = memidx(idx[0], idx[1]+1);
			const uint idx2 = idx1 + d[0];

			if (idx1 >= size()-1 || idx2 >= size()-1)
				return 0.0f;

			return bilin_interp(v[idx1], v[idx1+1], v[idx2], v[idx2+1], w[0], w[1]);
		}



		void setzero(){memset(v, 0, s*sizeof(T));}

		void set_domain(const intvlv& domain){
			dd = domain;
			gtod = linmapv(intvlv{vect2(), vect2{static_cast<T>(d[0]-1), static_cast<T>(d[1]-1)}}, domain);
			dtog = inv(gtod);
		}

		bool out_bound(const vect2& pos) const{
			const float eps = 0.5f;
			const vect2 off = eps * gtod.slope;
			return ( pos[0] <= dd.min[0] + off.x || pos[0] >= dd.max[0] - off.x || pos[1] <= dd.min[1] + off.y || pos[1] >= dd.max[1] - off.y );
		}


	private:
		T* v; // vector with samples
		vecu2 d; // grid dimension
		uint s; // grid size
		intvlv dd; // domain of definition/space
		linmapv gtod; // map grid to domain
		linmapv dtog; // map domain to grid


	public:
		uint memidx(uint i, uint j) const{return j * d[0] + i;}
		uint memidx(const vecu2& idx) const{return idx[1] * d[0] + idx[0];}


		template<class N>
		friend void swap_data(field<N>& f, field<N>& g);
		template<class N>
		friend void swap(field<N>& f, field<N>& g);


	private:
		void init_data(){
			v = new T[s];
		}

		void free_data(){
			if (v != nullptr) delete[] v;
		}

		void copy_head(const field& f){
			d = f.d;
			s = f.s;
			dd = f.dd;
			gtod = f.gtod;
			dtog = f.dtog;
		}

		void copy_data(const field& f){
			memcpy(v, f.v, s*sizeof(T));
		}

		void swap_data(field& f){
			T* const tmp = f.v;
			f.v = v;
			v = tmp;
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

	const vec<unsigned int,2> tmp2 = g.d;
	g.d = f.d;
	f.d = tmp2;

	swap_data(f,g);
}


} // xmath


#endif // __FIELD_H__