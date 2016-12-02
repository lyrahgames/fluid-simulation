#include "cfs.h"


void CFS::poisson_test_jacobi_it(){
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

void CFS::wave_it(){
	tmp.resize(p.v.size(), 0.0f);


	// const float damp = 10.0f;
	// const float c = 2.0f;
	const float dt = wave_dt;
	const float inv_dt = 1.0f / dt;
	const float sq_inv_dt = xmath::op::sq(inv_dt);
	const float factor = 1.0f / (sq_inv_dt + 0.5f * wave_damp * inv_dt);
	const vecf2 sq_inv_step = sq(p.stoi.slope);

	for (uint j = 1; j < p.size[1]-1; j++){
		for (uint i = 1; i < p.size[0]-1; i++){
			tmp[p.memidx(i,j)] = 
				factor * (
					wave_c * ( sq_inv_step.x * ( p(i+1,j) - 2.0f*p(i,j) + p(i-1,j) ) + 
					sq_inv_step.y * ( p(i,j+1) - 2.0f*p(i,j) + p(i,j-1) ) ) +
					sq_inv_dt * ( 2.0f*p(i,j) - p_old[p.memidx(i,j)] ) +
					0.5f * wave_damp * inv_dt * ( p_old[p.memidx(i,j)] )
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

void CFS::grad(){
	for (uint i = 0; i < vx.size[0]; i++){
		for (uint j = 0; j < vx.size[1]; j++){
			const vecf2 pos = vx.itos(vecf2{(float)i,(float)j});
			vx(i,j) = p.stoi.slope.x * ( p(i+1,j) - p(i,j) );
		}
	}

	for (uint i = 0; i < vy.size[0]; i++){
		for (uint j = 0; j < vy.size[1]; j++){
			const vecf2 pos = vy.itos(vecf2{(float)i,(float)j});
			vy(i,j) = p.stoi.slope.y * ( p(i,j+1) - p(i,j) );
		}
	}
}

void CFS::compute_dt(){
	const vecf2 sq_inv_step = sq(p.stoi.slope);
	dt = 0.5f * time_safe * reynold / (sq_inv_step.x + sq_inv_step.y);
}

void CFS::set_v_bound(){
	// const float w = 1.0f;

	// for (uint i = 0; i < vx.size[0]; i++){
	// 	vy(i,0) = 0.0f;
	// 	vy(i,vy.size[1]-1) = 0.0f;

	// 	vx(i,0) = 2.0f*w - vx(i,1);
	// 	vx(i,vx.size[1]-1)= -vx(i,vx.size[1]-2);
	// }

	// vy(vy.size[0]-1,0) = 0.0f;
	// vy(vy.size[0]-1,vy.size[1]-1) = 0.0f;

	// for (uint j = 1; j < vy.size[1]-1; j++){
	// 	vx(0,j) = 0.0f;
	// 	vx(vx.size[0]-1,j) = 0.0f;

	// 	vy(0,j) = -vy(1,j);
	// 	vy(vy.size[0]-1,j) = -vy(vy.size[0]-2,j);
	// }

	// vx(0,vx.size[1]-1) = 0.0f;
	// vx(vx.size[0]-1, vx.size[1]-1) = 0.0f;


	for (uint i = 0; i < vx_idx_persis.size(); i++){
		vx.v[vx_idx_persis[i]] = vx_persis[i];
	}
}

void CFS::compute_poisson_rhs(){
	const float inv_reynold = 1.0f/reynold;
	const vecf2 sq_inv_step = sq(vx.stoi.slope);
	const float inv_dx = vx.stoi.slope.x;
	const float inv_dy = vx.stoi.slope.y;
	const float inv_dt = 1.0f/dt;

	for (uint j = 1; j < vx.size[1]-1; j++){
		for (uint i = 1; i < vx.size[0]-1; i++){

			const float tmp1 = (
				sq_inv_step.x * ( vx(i+1,j) - 2.0f*vx(i,j) + vx(i-1,j) ) +
				sq_inv_step.y * ( vx(i,j+1) - 2.0f*vx(i,j) + vx(i,j-1) ) 
			);

			const float tmp2 = 0.25f * inv_dx * (
				deriv_weight * (
					xmath::op::sq( vx(i,j) + vx(i+1,j) ) -
					xmath::op::sq( vx(i-1,j) + vx(i,j) )
				) + 
				(1.0f - deriv_weight) * (
					fabsf(vx(i,j) + vx(i+1,j)) * (vx(i,j) - vx(i+1,j)) -
					fabsf(vx(i-1,j) + vx(i,j)) * (vx(i-1,j) - vx(i,j)) 
				)
			);

			const float tmp3 = 0.25f * inv_dy * (
				deriv_weight * (
					(vy(i,j) + vy(i+1,j)) * (vx(i,j) + vx(i,j+1)) -
					(vy(i,j-1) + vy(i+1,j-1)) * (vx(i,j-1) + vx(i,j))
				) + 
				(1.0f - deriv_weight) * (
					fabsf(vy(i,j) + vy(i+1,j)) * (vx(i,j) - vx(i,j+1)) -
					fabsf(vy(i,j-1) + vy(i+1,j-1)) * (vx(i,j-1) - vx(i,j)) 
				)
			);

			vx_tmp[vx.memidx(i,j)] = vx(i,j) + dt * (inv_reynold * tmp1 - tmp2 - tmp3 + force.x);
		}
	}

	for (uint j = 1; j < vy.size[1]-1; j++){
		for (uint i = 1; i < vy.size[0]-1; i++){

			const float tmp1 = (
				sq_inv_step.x * ( vy(i+1,j) - 2.0f*vy(i,j) + vy(i-1,j) ) +
				sq_inv_step.y * ( vy(i,j+1) - 2.0f*vy(i,j) + vy(i,j-1) ) 
			);

			const float tmp2 = 0.25f * inv_dy * (
				deriv_weight * (
					xmath::op::sq( vy(i,j) + vy(i,j+1) ) -
					xmath::op::sq( vy(i,j-1) + vy(i,j) )
				) + 
				(1.0f - deriv_weight) * (
					fabsf(vy(i,j) + vy(i,j+1)) * (vy(i,j) - vy(i,j+1)) -
					fabsf(vy(i,j-1) + vy(i,j)) * (vy(i,j-1) - vy(i,j)) 
				)
			);

			const float tmp3 = 0.25f * inv_dx * (
				deriv_weight * (
					(vy(i,j) + vy(i+1,j)) * (vx(i,j) + vx(i,j+1)) -
					(vx(i-1,j) + vx(i-1,j+1)) * (vy(i-1,j) + vy(i,j))
				) + 
				(1.0f - deriv_weight) * (
					fabsf(vx(i,j) + vx(i,j+1)) * (vy(i,j) - vy(i+1,j)) -
					fabsf(vx(i-1,j) + vx(i-1,j+1)) * (vy(i-1,j) - vy(i,j)) 
				)

			);

			vy_tmp[vy.memidx(i,j)] = vy(i,j) + dt * (inv_reynold * tmp1 - tmp2 - tmp3 + force.y);
		}
	}

	for (uint j = 1; j < p.size[1]-1; j++){
		for (uint i = 1; i < p.size[0]-1; i++){
			rhs[p.memidx(i,j)] = inv_dt * (
				inv_dx * ( vx_tmp[vx.memidx(i,j)] - vx_tmp[vx.memidx(i-1,j)] ) +
				inv_dy * ( vy_tmp[vy.memidx(i,j)] - vy_tmp[vy.memidx(i,j-1)] )
			);
		}
	}
}

void CFS::poisson_p_sor_it(){
	const vecf2 sq_inv_step = sq(p.stoi.slope);

	// const float lambda = -2.0f;
	// const float bound = 1.0f;

	const float factor = 1.0f / ( 2.0f*(sq_inv_step.x + sq_inv_step.y) );
	const float factor_x = 1.0f / (sq_inv_step.x + 2.0f*sq_inv_step.y);
	const float factor_y = 1.0f / (2.0f*sq_inv_step.x + sq_inv_step.y);
	const float factor_p = 1.0f / (sq_inv_step.x + sq_inv_step.y);

	for	(uint it = 0; it < sor_it_max; it++){

		p(0.0) = (1.0f - sor_relax) * p(0,0) + 
			sor_relax * factor_p * (
				sq_inv_step.x * p(1,0) +
				sq_inv_step.y * p(0,1) -
				rhs[p.memidx(0.0)]
			);

		for (uint i = 1; i < p.size[0]-1; i++){
			p(i,0) = (1.0f - sor_relax) * p(i,0) +
				sor_relax * factor_y * (
					sq_inv_step.x * ( p(i+1,0) + p(i-1,0) ) +
					sq_inv_step.y * p(i,1) -
					rhs[p.memidx(i,0)]
				);
		}

		p(p.size[0]-1,0) = (1.0f - sor_relax) * p(p.size[0]-1,0) +
			sor_relax * factor_p * (
				sq_inv_step.x * p(p.size[0]-2, 0) +
				sq_inv_step.y * p(p.size[0]-1, 1) -
				rhs[p.memidx(p.size[0]-1,0)]
			);

		for (uint j = 1; j < p.size[1]-1; j++){ // field is stored in row-major order, so j has to be first

			p(0,j) = (1.0f - sor_relax) * p(0,j) +
				sor_relax * factor_x * (
					sq_inv_step.x * p(1,j) +
					sq_inv_step.y * ( p(0,j+1) + p(0,j-1) ) -
					rhs[p.memidx(0,j)]
				);

			for (uint i = 1; i < p.size[0]-1; i++){
				// p(i,j) will be overwritten after computation (advantage of sor)
				p(i,j) = (1.0f - sor_relax) * p(i,j) +
					sor_relax * factor * (
						sq_inv_step.x * (p(i+1,j) + p(i-1,j)) +
						sq_inv_step.y * (p(i,j+1) + p(i,j-1)) -
						rhs[p.memidx(i,j)]
					);
			}

			p(p.size[0]-1,j) = (1.0f - sor_relax) * p(p.size[0]-1,j) +
				sor_relax * factor_x * (
					sq_inv_step.x * p(p.size[0]-2,j) +
					sq_inv_step.y * ( p(p.size[0]-1,j+1) + p(p.size[0]-1,j-1) ) -
					rhs[p.memidx(p.size[0]-1,j)]
				);
		}

		p(0, p.size[1]-1) = (1.0f - sor_relax) * p(0,p.size[1]-1) +
			sor_relax * factor_p * (
				sq_inv_step.x * p(1, p.size[1]-1) +
				sq_inv_step.y * p(0, p.size[1]-2) -
				rhs[p.memidx(0, p.size[1]-1)]
			);

		for (uint i = 1; i < p.size[0]-1; i++){			
			p(i,p.size[1]-1) = (1.0f - sor_relax) * p(i,p.size[1]-1) +
				sor_relax * factor_y * (
					sq_inv_step.x * ( p(i+1,p.size[1]-1) + p(i-1,p.size[1]-1) ) +
					sq_inv_step.y * p(i,p.size[1]-2) -
					rhs[p.memidx(i,p.size[1]-1)]
				);
		}

		p(p.size[0]-1, p.size[1]-1) = (1.0f - sor_relax) * p(p.size[0]-1, p.size[1]-1) +
			sor_relax * factor_p * (
				sq_inv_step.x * p(p.size[0]-2, p.size[1]-1) +
				sq_inv_step.y * p(p.size[0]-1, p.size[1]-2) -
				rhs[p.memidx(p.size[0]-1, p.size[1]-1)]
			);
	}	
}

void CFS::poisson_p_jacobi_it(){
	tmp.resize(p.v.size(), 0.0f);

	const vecf2 sq_inv_step = sq(p.stoi.slope);

	const float factor = 1.0f / ( 2.0f*(sq_inv_step.x + sq_inv_step.y) );
	const float factor_x = 1.0f / (sq_inv_step.x + 2.0f*sq_inv_step.y);
	const float factor_y = 1.0f / (2.0f*sq_inv_step.x + sq_inv_step.y);
	const float factor_p = 1.0f / (sq_inv_step.x + sq_inv_step.y);

	for	(uint it = 0; it < jacobi_it_max; it++){
		for (uint j = 1; j < p.size[1]-1; j++){
			tmp[p.memidx(0,j)] = 
			factor_x * (
				sq_inv_step.x * p(1,j) +
				sq_inv_step.y * ( p(0,j+1) + p(0,j-1) ) -
				rhs[p.memidx(0,j)]
			);
			
			tmp[p.memidx(p.size[0]-1,j)] = 
			factor_x * (
				sq_inv_step.x * p(p.size[0]-2,j) +
				sq_inv_step.y * ( p(p.size[0]-1,j+1) + p(p.size[0]-1,j-1) ) -
				rhs[p.memidx(p.size[0]-1,j)]
			);
		}

		for (uint i = 1; i < p.size[0]-1; i++){
			tmp[p.memidx(i,0)] = 
			factor_y * (
				sq_inv_step.x * ( p(i+1,0) + p(i-1,0) ) +
				sq_inv_step.y * p(i,1) -
				rhs[p.memidx(i,0)]
			);
			
			tmp[p.memidx(i,p.size[1]-1)] = 
			factor_y * (
				sq_inv_step.x * ( p(i+1,p.size[1]-1) + p(i-1,p.size[1]-1) ) +
				sq_inv_step.y * p(i,p.size[1]-2) -
				rhs[p.memidx(i,p.size[1]-1)]
			);
		}

		for (uint j = 1; j < p.size[1]-1; j++){
			for (uint i = 1; i < p.size[0]-1; i++){
				tmp[p.memidx(i,j)] = factor * (
					sq_inv_step.x * (p(i+1,j) + p(i-1,j)) +
					sq_inv_step.y * (p(i,j+1) + p(i,j-1)) -
					rhs[p.memidx(i,j)]
				);
			}
		}

		p.v.swap(tmp);
	}
}

void CFS::compute_v(){
	const float inv_dx = vx.stoi.slope.x;
	const float inv_dy = vx.stoi.slope.y;

	for (uint j = 1; j < vx.size[1]-1; j++){
		for (uint i = 1; i < vx.size[0]-1; i++){
			vx(i,j) = vx_tmp[vx.memidx(i,j)] - dt * inv_dx * (p(i+1,j) - p(i,j));
		}
	}

	for (uint j = 1; j < vy.size[1]-1; j++){
		for (uint i = 1; i < vy.size[0]-1; i++){
			vy(i,j) = vy_tmp[vy.memidx(i,j)] - dt * inv_dy * (p(i,j+1) - p(i,j));
		}
	}
}

void CFS::compute_time_it(){
	compute_dt();
	set_v_bound();
	compute_poisson_rhs();
	// poisson_p_jacobi_it();
	poisson_p_sor_it();
	compute_v();

	time += dt;
	it_step++;
}