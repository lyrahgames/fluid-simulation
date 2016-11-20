#ifndef __CFS_H__
#define __CFS_H__

#include <util.h>


struct CFS{
	struct field{
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
			space(range), p(field(width, height, range)){}


		// void poisson_sor(){
			
		// }

	public:
		intvlv space;
		field p;
};


#endif // __CFS_H__