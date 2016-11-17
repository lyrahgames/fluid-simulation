#include <cfs.h>


void CFS::set_v_bound(){
	const float w = 1.0f;

	for (uint i = 0; i < vx_width(); i++){
		_v[1][vy_memidx(i,0)] = 0.0f;
		_v[1][vy_memidx(i,vy_height()-1)] = 0.0f;

		_v[0][vx_memidx(i,0)] = 2.0f*w - _v[0][vx_memidx(i,1)];
		_v[0][vx_memidx(i,vx_height()-1)] = -_v[0][vx_memidx(i,vx_height()-2)];
	}

	_v[1][vy_memidx(vy_width()-1,0)] = 0.0f;
	_v[1][vy_memidx(vy_width()-1,vy_height()-1)] = 0.0f;

	for (uint j = 1; j < vy_height()-1; j++){
		_v[0][vx_memidx(0,j)] = 0.0f;
		_v[0][vx_memidx(vx_width()-1,j)] = 0.0f;

		_v[1][vy_memidx(0,j)] = -_v[1][vy_memidx(1,j)];
		_v[1][vy_memidx(vy_width()-1,j)] = -_v[1][vy_memidx(vy_width()-2,j)];
	}

	_v[0][vx_memidx(0,vx_height()-1)] = 0.0f;
	_v[0][vx_memidx(vx_width()-1, vx_height()-1)] = 0.0f;
}

void CFS::compute_equ(){
	for (sint i = 1; i < _grid_map[0].size() - 1; i++){
		for (sint j = 1; j < _grid_map[1].size() - 1; j++){
			// const sint idx = _grid_map[0].size() * i + j;


			const uint p_idx = p_memidx(i,j);

			const uint vx_idx = vx_memidx(i,j);
			const uint vx_idx_l = vx_memidx_l(vx_idx);
			const uint vx_idx_r = vx_memidx_r(vx_idx);
			const uint vx_idx_d = vx_memidx_d(vx_idx);
			const uint vx_idx_u = vx_memidx_u(vx_idx);
			const uint vx_idx_dr = vx_memidx_r(vx_idx_d);

			const uint vy_idx = vy_memidx(i,j);
			const uint vy_idx_l = vy_memidx_l(vy_idx);
			const uint vy_idx_r = vy_memidx_r(vy_idx);
			const uint vy_idx_d = vy_memidx_d(vy_idx);
			const uint vy_idx_u = vy_memidx_u(vy_idx);
			const uint vy_idx_dr = vy_memidx_r(vy_idx_d);

			const float sq_inv_dx = sq(_grid_map[0].inv_step());
			const float sq_inv_dy = sq(_grid_map[1].inv_step());

			const float cdq_weight_1 = 1.0f - cdq_weight();


			const float tmp11 = inv_reynold() * (
				sq_inv_dx * ( _v[0][vx_idx_r] - 2.0f*_v[0][vx_idx] + _v[0][vx_idx_l] ) +
				sq_inv_dy * ( _v[0][vx_idx_u] - 2.0f*_v[0][vx_idx] + _v[0][vx_idx_d] ) 
			);
			if (fabsf(tmp11) > 0.001f)
				printf("%.8f\n",tmp11);

			const float tmp12 = 0.25f * _grid_map[0].inv_step() * (
				cdq_weight() * (
					sq( _v[0][vx_idx] + _v[0][vx_idx_r]) -
					sq( _v[0][vx_idx_l] + _v[0][vx_idx])
				) + 
				cdq_weight_1 * (
					fabsf(_v[0][vx_idx] + _v[0][vx_idx_r]) * (_v[0][vx_idx] - _v[0][vx_idx_r]) -
					fabsf(_v[0][vx_idx_l] + _v[0][vx_idx]) * (_v[0][vx_idx_l] - _v[0][vx_idx]) 
				)

			);

			const float tmp13 = 0.25f * _grid_map[1].inv_step() * (
				cdq_weight() * (
					(_v[1][vy_idx] + _v[1][vy_idx_r]) * (_v[0][vx_idx] + _v[0][vx_idx_u]) -
					(_v[1][vy_idx_d] + _v[1][vy_idx_dr]) * (_v[0][vx_idx_d] + _v[0][vx_idx])
				) + 
				cdq_weight_1 * (
					fabsf(_v[1][vy_idx] + _v[1][vy_idx_r]) * (_v[0][vx_idx] - _v[0][vx_idx_u]) -
					fabsf(_v[1][vy_idx_d] + _v[1][vy_idx_dr]) * (_v[0][vx_idx_d] - _v[0][vx_idx]) 
				)

			);

			_tmp[0][vx_idx] = _v[0][vx_idx] + time_step() * (tmp11 - tmp12 - tmp13 + force()[0]);






			// const float F_tmp1 = Re_inv * ( (u(i+1,j) - 2*u(i,j) + u(i-1,j))/(grid.scale().x * grid.scale().x) + (u(i,j+1) - 2*u(i,j) + u(i,j-1))/(grid.scale().y * grid.scale().y) );

			// const float F_tmp2 = grid.invScale().x * ( sq( 0.5f * (u(i,j) + u(i+1,j)) ) - sq( 0.5f * (u(i-1,j) + u(i,j)) ) )
			// 					+ gamma * grid.invScale().x * 0.25f * ( fabs(u(i,j) + u(i+1,j)) * (u(i,j) - u(i+1,j))  -  fabs(u(i-1,j) + u(i,j)) * (u(i-1,j) - u(i,j)) );

			// const float F_tmp3 = grid.invScale().y * 0.25f * ( (v(i,j) + v(i+1,j))*(u(i,j) + u(i,j+1)) - (v(i,j-1) + v(i+1,j-1))*(u(i,j-1) + u(i,j)) )
			// 					+ gamma * grid.invScale().y * 0.25f * ( fabs(v(i,j) + v(i+1,j))*(u(i,j) - u(i,j+1)) - fabs(v(i,j-1) + v(i+1,j-1))*(u(i,j-1) - u(i,j)) );

			// F(i,j) = u(i,j) + dt * ( F_tmp1 - F_tmp2 - F_tmp3 + g.x );





			const float tmp21 = inv_reynold() * (
				sq_inv_dx * ( _v[1][vy_idx_r] - 2.0f*_v[1][vy_idx] + _v[1][vy_idx_l] ) +
				sq_inv_dy * ( _v[1][vy_idx_u] - 2.0f*_v[1][vy_idx] + _v[1][vy_idx_d] ) 
			);

			const float tmp22 = 0.25f * _grid_map[1].inv_step() * (
				cdq_weight() * (
					sq( _v[1][vy_idx] + _v[1][vy_idx_r]) -
					sq( _v[1][vy_idx_l] + _v[1][vy_idx])
				) + 
				cdq_weight_1 * (
					fabsf(_v[1][vy_idx] + _v[1][vy_idx_r]) * (_v[1][vy_idx] - _v[1][vy_idx_r]) -
					fabsf(_v[1][vy_idx_l] + _v[1][vy_idx]) * (_v[1][vy_idx_l] - _v[1][vy_idx]) 
				)

			);

			const float tmp23 = 0.25f * _grid_map[0].inv_step() * (
				cdq_weight() * (
					(_v[0][vx_idx] + _v[0][vx_idx_r]) * (_v[1][vy_idx] + _v[1][vy_idx_u]) -
					(_v[0][vx_idx_d] + _v[0][vx_idx_dr]) * (_v[1][vy_idx_d] + _v[1][vy_idx])
				) + 
				cdq_weight_1 * (
					fabsf(_v[0][vx_idx] + _v[0][vx_idx_r]) * (_v[1][vy_idx] - _v[1][vy_idx_u]) -
					fabsf(_v[0][vx_idx_d] + _v[0][vx_idx_dr]) * (_v[1][vy_idx_d] - _v[1][vy_idx]) 
				)

			);

			_tmp[1][vy_idx] = _v[1][vy_idx] + time_step() * (tmp21 - tmp22 - tmp23 + force()[1]);





			// const float G_tmp1 = Re_inv * ( (v(i+1,j) - 2*v(i,j) + v(i-1,j))/(grid.scale().x * grid.scale().x) + (v(i,j+1) - 2*v(i,j) + v(i,j-1))/(grid.scale().y * grid.scale().y) );
			// const float G_tmp2 = grid.invScale().y * ( sq( 0.5f * (v(i,j) + v(i,j+1)) ) - sq( 0.5f * (v(i,j-1) + v(i,j)) ) )
			// 					+ gamma * grid.invScale().y * 0.25f * ( fabs(v(i,j) + v(i,j+1)) * (v(i,j) - v(i,j+1))  -  fabs(v(i,j-1) + v(i,j)) * (v(i,j-1) - v(i,j)) );
			// const float G_tmp3 = grid.invScale().x * 0.25f * ( (v(i,j) + v(i+1,j))*(u(i,j) + u(i,j+1)) - (u(i-1,j) + u(i-1,j+1))*(v(i-1,j) + v(i,j)) )
			// 					+ gamma * grid.invScale().x * 0.25f * ( fabs(u(i,j) + u(i,j+1))*(v(i,j) - v(i+1,j)) - fabs(u(i-1,j) + u(i-1,j+1))*(v(i-1,j) - v(i,j)) );
			// G(i,j) = v(i,j) + dt * ( G_tmp1 - G_tmp2 - G_tmp3 + g.y );




			_tmp[2][p_idx] = inv_time_step() * (
				_grid_map[0].inv_step() * (_tmp[0][vx_idx] - _tmp[0][vx_idx_l]) +
				_grid_map[1].inv_step() * (_tmp[1][vy_idx] - _tmp[1][vy_idx_d])
			); 



			// RHS(i,j) = inv_dt * ( grid.invScale().x * (F(i,j) - F(i-1,j)) + grid.invScale().y * (G(i,j) - G(i,j-1)) );
		}
	}
}


void CFS::compute_poisson(){
	float res;
	uint it = 0;

	do{
		for (uint i = 0; i < p_width(); i++){
			uint p_idx = p_memidx(i,0);
			_p[p_idx] = _p[p_memidx_u(p_idx)];

			p_idx = p_memidx(i, _grid_map[1].size()-1);
			_p[p_idx] = _p[p_memidx_d(p_idx)];
		}

		for (uint j = 1; j < p_height()-1; j++){
			uint p_idx = p_memidx(0,j);
			_p[p_idx] = _p[p_memidx_r(p_idx)];

			p_idx = p_memidx(_grid_map[0].size()-1, j);
			_p[p_idx] = _p[p_memidx_l(p_idx)];
		}

		const float sq_inv_dx = sq(_grid_map[0].inv_step());
		const float sq_inv_dy = sq(_grid_map[1].inv_step());


		// change order of for loops for better caching
		for (uint i = 1; i < p_width()-1; i++){
			for (uint j = 1; j < p_height()-1; j++){
				const uint p_idx = p_memidx(i,j);
				const uint p_idx_r = p_memidx_r(p_idx);
				const uint p_idx_l = p_memidx_l(p_idx);
				const uint p_idx_u = p_memidx_u(p_idx);
				const uint p_idx_d = p_memidx_d(p_idx);

				_p[p_idx] = (1.0f-sor_weight()) * _p[p_idx] +
					0.5f * sor_weight() * (
						sq_inv_dx * (_p[p_idx_r] + _p[p_idx_l]) + 
						sq_inv_dy * (_p[p_idx_d] + _p[p_idx_u]) -
						_tmp[2][p_idx]
					) / ( sq_inv_dx + sq_inv_dy );
			}
		}


		res = 0.0f;

		for (uint i = 1; i < p_width()-1; i++){
			for (uint j = 1; j < p_height()-1; j++){
				const uint p_idx = p_memidx(i,j);
				const uint p_idx_r = p_memidx_r(p_idx);
				const uint p_idx_l = p_memidx_l(p_idx);
				const uint p_idx_u = p_memidx_u(p_idx);
				const uint p_idx_d = p_memidx_d(p_idx);

				const float tmp_res = fabsf(
					sq_inv_dx * (_p[p_idx_r] - 2.0f*_p[p_idx] + _p[p_idx_l]) + 
					sq_inv_dy * (_p[p_idx_u] - 2.0f*_p[p_idx] + _p[p_idx_d]) -
					_tmp[2][p_idx]
				);

				if (tmp_res > res){
					res = tmp_res;
				}
			}
		}

		it++;
	}while(res >= sor_eps() && it < sor_it_max());
}

void CFS::compute_v(){
	for (uint j = 1; j < _grid_map[1].size()-1; j++){
		for (uint i = 1; i < _grid_map[0].size()-1; i++){
			const uint vx_idx = vx_memidx(i,j);
			const uint vy_idx = vy_memidx(i,j);
			const uint p_idx = p_memidx(i,j);
			const uint p_idx_r = p_memidx_r(p_idx);
			// const uint p_idx_l = p_memidx_l(p_idx);
			const uint p_idx_u = p_memidx_u(p_idx);
			// const uint p_idx_d = p_memidx_d(p_idx);

			_v[0][vx_idx] = _tmp[0][vx_idx] - time_step() * _grid_map[0].inv_step() * (_p[p_idx_r] - _p[p_idx]);
			_v[1][vy_idx] = _tmp[1][vy_idx] - time_step() * _grid_map[1].inv_step() * (_p[p_idx_u] - _p[p_idx]);
		}
	}
}

void CFS::compute_time_it(){
	compute_time_step_bound();
	compute_time_step();
	// printf("bound:\t%f\n", time_step_bound());
	// printf("dt:\t%f\n", time_step());
	// we need boundaries

	set_v_bound();

	compute_equ();
	compute_poisson();
	compute_v();
	_time += time_step();
	_it_step++;
}