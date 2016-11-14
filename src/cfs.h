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

			for (int i = 0; i < p_size(); i++){
				_p[i] = sq(sin(10.0f * float(i)/float(p_size())));
			}
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


		float p(float in_x, float in_y) const{
			if (pos_out_p_bound(in_x, in_y))
				return 0.0f;

			const float x = _grid_map[0].cell_idx(in_x);
			const float y = _grid_map[1].cell_idx(in_y);
			const float fidx_x = floor(x);
			const float fidx_y = floor(y);
			const uint idx_x = static_cast<uint>(fidx_x);
			const uint idx_y = static_cast<uint>(fidx_y);
			const float w_x = x - fidx_x;
			const float w_y = y - fidx_y;
			const uint idx1 = p_memidx(idx_x, idx_y);
			const uint idx2 = p_memidx(idx_x, idx_y+1);

			return bilin_interp(_p[idx1], _p[idx1+1], _p[idx2], _p[idx2+1], w_x, w_y);
		}


		// void compute_time_it();

		void compute_time_step(){
			const float tmp1 = _grid_map[0].step() * _inv_v_max_mag[0];
			const float tmp2 = _grid_map[1].step() * _inv_v_max_mag[1];

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
			const float tmp = (1.0f / sq(_grid_map[0].step())) + (1.0f / sq(_grid_map[1].step()));
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
		uint p_size() const{return p_width()*p_height();}
		uint p_width() const{return _grid_map[0].cell_count();}
		uint p_height() const{return _grid_map[1].cell_count();}
		uint vx_size() const{return vx_width()*vx_height();}
		uint vx_width() const{return _grid_map[0].cell_count() - 1;}
		uint vx_height() const{return _grid_map[1].cell_count();}
		uint vy_size() const{return vy_width()*vy_height();}
		uint vy_width() const{return _grid_map[0].cell_count();}
		uint vy_height() const{return _grid_map[1].cell_count() - 1;}

		uint p_memidx(uint idx_x, uint idx_y) const{
			return quad_rm_memidx(p_width(), idx_y, idx_x);
		}
		uint vx_memidx(uint idx_x, uint idx_y) const{
			return quad_rm_memidx(vx_width(), idx_y, idx_x);
		}
		uint vy_memidx(uint idx_x, uint idx_y) const{
			return quad_rm_memidx(vy_height(), idx_y, idx_x);
		}

		void init_field(){
			_p.resize(p_size(), 0.0f);
			_v[0].resize(vx_size(), 0.0f);
			_v[1].resize(vy_size(), 0.0f);
		}

		bool pos_out_bound(float in_x, float in_y) const{
			return (in_x < _grid_map[0].min() || in_x > _grid_map[0].max() || in_y < _grid_map[1].min() || in_y > _grid_map[1].max());
		}

		bool pos_out_p_bound(float in_x, float in_y) const{
			return (in_x <= _grid_map[0].cell_pos(0) || in_x >= _grid_map[0].cell_pos(p_width()-1) || in_y <= _grid_map[1].cell_pos(0) || in_y >= _grid_map[1].cell_pos(p_height()-1));
		}
};


#endif // __CFS_H__