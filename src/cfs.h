#ifndef __CFS_H__
#define __CFS_H__

#include <util.h>


struct CFS{
	struct field{
		vectorf v;
		vecu2 size;
		linmapv gm; // gridmap (grid --> space)
		linmapv sm; // spacemap (space --> grid)

		field(const intvlv& domain, const intvlv& range):
			size(len(domain)),
			v(vectorf(w*h, 0.0f)),
			gm(linmapv(domain, range)),
			sm(linmapv(range, domain)){}


		float& operator[](uint idx){return v[idx];}
		constexpr const float& operator[](uint idx) const{return v[idx];}


		float& operator()(uint i, uint j){
			return v[j*size[0] + i];
		}

		constexpr const float& operator()(uint i, uint j) const{
			return v[j*size[0] + i];
		}


		float operator()(float x, float y) const{
			const float mx = sm(x);
			const float my = sm(y);
			const float mi = floor(mx);
			const float mj = floor(my);
			const uint i = static_cast<uint>(mi);
			const uint j = static_cast<uint>(mj);
			const float wi = mx - mi;
			const float wj = my - mj;
			// const uint idx1 = p_memidx(idx_x, idx_y);
			// const uint idx2 = p_memidx(idx_x, idx_y+1);

			return bilin_interp(v(i,j), v(i+1,j), v(i,j+1), v(i+1,j+1), wi, wj);
		}
	};


	public:
		field p;
};


#endif // __CFS_H__