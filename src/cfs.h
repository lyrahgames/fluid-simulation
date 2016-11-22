#ifndef __CFS_H__
#define __CFS_H__

#include <util.h>


struct CFS{
	struct field{ // public for debugging
		vectorf v;
		vecu2 size;
		intvlv space;
		linmapv itos; // map (index --> space)
		linmapv stoi; // map (space --> index)


		field() = default; // for tests end debugging
		field(uint width, uint height, const intvlv& viewport):
			space(viewport),
			size(vecu2{width, height}),
			v(vectorf(width*height, 0.0f)),
			itos(linmapv(intvlv{vecf2(), vecf2(width-1, height-1)}, viewport)),
			stoi(inv(itos)){}


		float& operator[](uint idx){return v[idx];}
		const float& operator[](uint idx) const{return v[idx];}

		uint memidx(uint i, uint j) const{return j*size[0] + i;}
		uint memidx(const vecu2& idx) const{return idx[1] * size[0] + idx[0];}

		float& operator()(uint i, uint j){return v[memidx(i,j)];}
		const float& operator()(uint i, uint j) const{return v[memidx(i,j)];}

		float& operator()(const vecu2& idx){return v[memidx(idx)];}
		const float& operator()(const vecu2& idx) const{return v[memidx(idx)];}

		bool out_bound(const vecu2& idx) const{
			return ( (idx[0] >= size[0]) || (idx[1] >= size[1]) );
		}

		bool out_bound(const vecf2& pos) const{
			const float eps = 0.5f;
			const vecf2 off = eps * itos.slope;
			return ( pos[0] <= space.min[0] + off.x || pos[0] >= space.max[0] - off.x || pos[1] <= space.min[1] + off.y || pos[1] >= space.max[1] - off.y );
		}

		float operator()(vecf2 pos) const{
			const vecf2 midx = stoi(pos);
			const vecf2 sidx = fl(midx);
			const vecu2 idx = sidx;
			const vecf2 w = midx - sidx;

			const uint idx1 = memidx(idx[0], idx[1]);
			const uint idx2 = memidx(idx[0], idx[1]+1);

			return bilin_interp(v[idx1], v[idx1+1], v[idx2], v[idx2+1], w[0], w[1]);
		}

		float save_call(const vecf2& pos) const{
			if (out_bound(pos))
				return 0.0f;

			return (*this)(pos);
		}
	};


	public:
		CFS(uint width, uint height, const intvlv& range):
			space(range), p(field(width, height, range)),
			tmp(p.v), p_old(p.v), rhs(p.v),
			jacobi_it_max(100), sor_it_max(1), sor_relax(1.5f),
			vx(field(width-1, height, intvlv{p.space.min + vecf2{0.5f * p.itos.slope.x, 0.0f}, p.space.max - vecf2{0.5f * p.itos.slope.x, 0.0f}})),
			vy(field(width, height-1, intvlv{p.space.min + vecf2{0.0f, 0.5f * p.itos.slope.y}, p.space.max - vecf2{0.0f, 0.5f * p.itos.slope.y}})),
			vx_tmp(vx.v), vy_tmp(vy.v),
			reynold(10.0f), deriv_weight(0.5f),
			force(vecf2(0.0f, 0.0f)),
			dt(0.001f),
			time(0.0f), it_step(0),
			wave_damp(10.0f), wave_c(2.0f), wave_dt(0.001f){}


		void clear(){
			std::fill(p.v.begin(), p.v.end(), 0.0f);
			std::fill(tmp.begin(), tmp.end(), 0.0f);
			std::fill(p_old.begin(), p_old.end(), 0.0f);
			std::fill(rhs.begin(), rhs.end(), 0.0f);
		}
		
		void poisson_test_jacobi_it(); // Δp = -λp
		void wave_it(); // c^2 Δp = ∂^2 p / ∂t^2 + γ∂p/∂t

		void compute_dt();
		void set_v_bound();
		void compute_poisson_rhs();
		void poisson_p_sor_it(); // Δp = rhs
		void poisson_p_jacobi_it();
		void compute_v();
		void compute_time_it();


	public:
		intvlv space;
		
		field p;
		vectorf tmp, p_old;
		vectorf rhs;

		uint jacobi_it_max;
		uint sor_it_max;
		float sor_relax;

		field vx, vy;
		vectorf vx_tmp, vy_tmp;

		float reynold;
		float deriv_weight;
		vecf2 force;
		float dt;
		float time;
		uint it_step;

		float wave_damp;
		float wave_c;
		float wave_dt;
};


#endif // __CFS_H__