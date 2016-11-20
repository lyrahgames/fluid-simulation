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
			space(range), p(field(width, height, range)), p_old(p.v){}


		void poisson_jacobi(){
			tmp.resize(p.v.size(), 0.0f);

			const vecf2 sq_inv_step = sq(p.stoi.slope);

			const float lambda = -2.0f;
			const float bound = 1.0f;
			const float factor = 1.0f / ( 2.0f*(sq_inv_step.x + sq_inv_step.y) - lambda );
			const float factor_x = 1.0f / (sq_inv_step.x + 2.0f*sq_inv_step.y - lambda);
			const float factor_y = 1.0f / (2.0f*sq_inv_step.x + sq_inv_step.y - lambda);

			for	(uint it = 0; it < jacobi_it_max; it++){
				for (uint j = 1; j < p.size[1]-1; j++){
					tmp[p.memidx(0,j)] = 
					// bound * sin(0.1f*j);
					factor_x * (
						sq_inv_step.x * p(1,j) +
						sq_inv_step.y * ( p(0,j+1) + p(0,j-1) )
					);
					
					tmp[p.memidx(p.size[0]-1,j)] = 
					// bound * sin(0.1f*j);
					factor_x * (
						sq_inv_step.x * p(p.size[0]-2,j) +
						sq_inv_step.y * ( p(p.size[0]-1,j+1) + p(p.size[0]-1,j-1) )
					);
				}

				for (uint i = 1; i < p.size[0]-1; i++){
					tmp[p.memidx(i,0)] = 
					// bound * cos(i);
					factor_y * (
						sq_inv_step.x * ( p(i+1,0) + p(i-1,0) ) +
						sq_inv_step.y * p(i,1)
					);
					
					tmp[p.memidx(i,p.size[1]-1)] = 
					// -bound * cos(i);
					factor_y * (
						sq_inv_step.x * ( p(i+1,p.size[1]-1) + p(i-1,p.size[1]-1) ) +
						sq_inv_step.y * p(i,p.size[1]-2)
					);
				}

				for (uint j = 1; j < p.size[1]-1; j++){
					for (uint i = 1; i < p.size[0]-1; i++){
						tmp[p.memidx(i,j)] = factor * (
							sq_inv_step.x * (p(i+1,j) + p(i-1,j)) +
							sq_inv_step.y * (p(i,j+1) + p(i,j-1))
						);
					}
				}

				p.v.swap(tmp);
			}
		}

		void wave(){
			tmp.resize(p.v.size(), 0.0f);


			const float damp = 10.0f;
			const float c = 2.0f;
			const float dt = 0.001f;
			const float inv_dt = 1.0f / dt;
			const float sq_inv_dt = xmath::op::sq(inv_dt);
			const float factor = 1.0f / (sq_inv_dt + 0.5f * damp * inv_dt);
			
			const vecf2 sq_inv_step = sq(p.stoi.slope);

			for (uint j = 1; j < p.size[1]-1; j++){
				for (uint i = 1; i < p.size[0]-1; i++){
					tmp[p.memidx(i,j)] = 
						factor * (
							c * ( sq_inv_step.x * ( p(i+1,j) - 2.0f*p(i,j) + p(i-1,j) ) + 
							sq_inv_step.y * ( p(i,j+1) - 2.0f*p(i,j) + p(i,j-1) ) ) +
							sq_inv_dt * ( 2.0f*p(i,j) - p_old[p.memidx(i,j)] ) +
							0.5f * damp * inv_dt * ( p_old[p.memidx(i,j)] )
						);
				}
			}

			// for (uint j = 1; j < p.size[1]-1; j++){
			// 	tmp[p.memidx(0,j)] = 
			// 		tmp[p.memidx(1,j)];
			// 		// tmp[p.memidx(p.size[0]-2,j)];
				
			// 	tmp[p.memidx(p.size[0]-1,j)] =
			// 		tmp[p.memidx(p.size[0]-2,j)];
			// 		// tmp[p.memidx(1,j)];
			// }

			// for (uint i = 1; i < p.size[0]-1; i++){
			// 	tmp[p.memidx(i,0)] = tmp[p.memidx(i,p.size[1]-2)];
				
			// 	tmp[p.memidx(i,p.size[1]-1)] = tmp[p.memidx(i,1)];
			// }

			p_old.swap(p.v);
			p.v.swap(tmp);
		}

	public:
		intvlv space;
		field p;
		vectorf tmp, p_old;
		uint jacobi_it_max;
		uint sor_it_max;
		float sor_weight;
};


#endif // __CFS_H__