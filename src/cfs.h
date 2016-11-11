#ifndef __CFS_H__
#define __CFS_H__

#include <cstring>
#include <vector>
#include <algorithm>

#include <vec.h>
#include <grid.h>
#include <util.h>


// cavity fluid simulation
struct CFS{
	using sfield = std::vector<float>;


	enum AXIS{
		X = 0, Y = 1
	};


	public:
		CFS(const gridmap& in_grid_map0, const gridmap& in_grid_map1):
		_grid_map{in_grid_map0, in_grid_map1},
		_p(p_size(), 0.0f),
		_v{sfield(vx_size(), 0.0f), sfield(vy_size(), 0.0f)}{

		}


		float reynold() const{return _reynold;}
		float inv_reynold() const{return _inv_reynold;}
		float time_step() const{return _time_step;}
		float inv_time_step() const{return _inv_time_step;}
		float time_step_safety() const{return _time_step_safety;}
		float time_step_bound() const{return _time_step_bound;}
		const gridmap* grid_map() const{return _grid_map;}
		// vec2 inv_v_max_mag() const{return _inv_v_max_mag;}
		// const sfield& p() const{return _p;}


		float p_bound(float in_x, float in_y) const{
			const float x = grid_map[0].cell_idx(in_x);
			const float y = grid_map[1].cell_idx(in_y);
			const float xidx = floor(x);
			const float yidx = floor(y);
			const float wx = x - xidx;
			const float wy = y - yidx;

			const uint idx1 = quad_rm_memidx(p_width, static_cast<uint>(yidx), static_cast<uint>(xidx));
			const uint idx2 = quad_rm_memidx(p_width, static_cast<uint>(yidx) + 1, static_cast<uint>(xidx));

			return bilin_interp(_p[idx1], _p[idx1+1], _p[idx2], _p[idx2+1], wx, wy);
		}


		// void compute_time_it();

		void compute_time_step(){
			const float tmp1 = _grid[0].scale() * _inv_v_max_mag[0];
			const float tmp2 = _grid[1].scale() * _inv_v_max_mag[1];

			_time_step = std::min(tmp1, tmp2);
			_time_step = std::min(_time_step_bound, time_step_safety() * _time_step);
			_inv_time_step = 1.0f / _time_step;
		}

		void set_time_step(float in_time_step){
			if (in_time_step <= 0.0f)
				return;

			_time_step = in_time_step;
			_inv_time_step = 1.0f / _time_step;
		}

		void compute_time_step_bound(){
			const float tmp = (1.0f / sq(_grid[0].scale())) + (1.0f / sq(_grid[1].scale()));
			_time_step_bound = 0.5f * time_step_safety() * reynold() / tmp;
		}


		// void compute_equ();

		void set_grid_map(const gridmap& in_grid_map0, const gridmap& in_grid_map1){
			_grid_map[0] = in_grid_map0;
			_grid_map[1] = in_grid_map1;

			init_field();
		}


	private:
		// underlying grid for pressure and velocity
		gridmap _grid_map[2];
		// pressure
		sfield _p;
		// velocity (structure of arrays)
		sfield _v[2];


		// parameters
		// reynold number
		float _reynold;
		// 
		float _time_step;
		// time step safety factor
		float _time_step_safety;
		// force/acceleration
		vec2 _force;
		// weight for donor-cell discretization
		float _donor_cell_weight;


		// temporary variables
		// maximum magnitude of velocity field
		vec2 _v_max_mag, _inv_v_max_mag;
		// inverse reynold number
		float _inv_reynold;
		// inverse time step
		float _inv_time_step;
		// upper boundary for adaptive time stepsize control
		float _time_step_bound;



	private:
		uint p_size(){return p_width*p_height;}
		uint p_width(){return _grid_map[0].cell_count();}
		uint p_height(){return _grid_map[1].cell_count();}
		uint vx_size(){return vx_width*vx_height;}
		uint vx_width(){return _grid_map[0].cell_count() - 1;}
		uint vx_height(){return _grid_map[1].cell_count();}
		uint vy_size(){return vy_width*vy_height;}
		uint vy_width(){return _grid_map[0].cell_count();}
		uint vy_height(){return _grid_map[1].cell_count() - 1;}

		void init_field(){
			_p.resize(p_size(), 0.0f);
			_v[0].resize(vx_size(), 0.0f);
			_v[1].resize(vy_size(), 0.0f);
		}

		bool pos_out_bound(float in_x, float in_y){
			return (in_x < grid_map[0].min() || in_x > grid_map[0].max() || in_y < grid_map[1].min() || in_y > grid_map[1].max());
		}
};


#endif // __CFS_H__