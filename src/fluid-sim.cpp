#include <fluid-sim.h>


fluid_sim::fluid_sim(uint width, uint height, const intvlv& geometry):
geom(geometry), dim(vecu2{width, height}),
p(fieldf(width,height,geometry)), p_tmp(fieldf(width,height,geometry)),
rhs(fieldf(width,height,geometry)), res(fieldf(width,height,geometry)),
res_eps(3.0f),
vx(fieldf(width-1,height,geometry)), vx_tmp(fieldf(width-1,height,geometry)),
vy(fieldf(width,height-1,geometry)), vy_tmp(fieldf(width,height-1,geometry)),
vx_max(0.0f), vx_min(0.0f), vy_max(0.0f), vy_min(0.0f),
t(0.0f), it(0),
dt_safe(0.5f),
re(1000.0f),
force(vecf2()),
deriv_w(0.5f),
jacobi_it_max(100),
jacobi_weight(1.0f),
sor_it_max(100),
sor_relax(0.5f){

	const intvlv vx_domain = {
		vecf2{geometry.min.x + 0.5f * p.grid_to_domain().slope.x, geometry.min.y},
		vecf2{geometry.max.x - 0.5f * p.grid_to_domain().slope.x, geometry.max.y} 
	};

	vx.set_domain(vx_domain);
	vx_tmp.set_domain(vx_domain);

	const intvlv vy_domain = {
		vecf2{geometry.min.x, geometry.min.y + 0.5f * p.grid_to_domain().slope.y},
		vecf2{geometry.max.x, geometry.max.y - 0.5f * p.grid_to_domain().slope.y}
	};

	vy.set_domain(vy_domain);
	vy_tmp.set_domain(vy_domain);

	init();
}

void fluid_sim::init(){
	clear();

	const vecf2 geom_len = len(geom);
	dx = geom_len.x / float(dim[0]-1);
	dy = geom_len.y / float(dim[1]-1);

	compute_grid_const();
	compute_dt_bound();

	inv_re = 1.0f / re;
}

void fluid_sim::clear(){
	p.setzero();
	p_tmp.setzero();
	rhs.setzero();
	res.setzero();
	vx.setzero();
	vx_tmp.setzero();
	vy.setzero();
	vy_tmp.setzero();
	vx_max = vx_min = 0.0f;
	vy_max = vy_min = 0.0f;
	t = 0.0f;
	it = 0;
}

void fluid_sim::compute_grid_const(){
	inv_dx = 1.0f / dx;
	inv_dy = 1.0f / dy;
	sq_inv_dx = inv_dx * inv_dx;
	sq_inv_dy = inv_dy * inv_dy;
	inner_const = 0.5f / (sq_inv_dx + sq_inv_dy);
	edge_x_const = 1.0f / (sq_inv_dx + 2.0f*sq_inv_dy);
	edge_y_const = 1.0f / (2.0f*sq_inv_dx + sq_inv_dy);
	vert_const = 2.0f * inner_const;
	res_l2_norm_const = sqrtf(1.0f / float(dim[0]*dim[1]));
}

void fluid_sim::compute_dt_bound(){
	dt_bound = dt_safe * re * inner_const;
}

void fluid_sim::compute_dt(){
	const float vx_abs_max = max(fabsf(vx_max), fabsf(vx_min));
	const float vy_abs_max = max(fabsf(vy_max), fabsf(vy_min));

	const float tmp = min(dx/vx_mag_max, dy/vy_mag_max);
	dt = min(dt_bound, dt_safe*tmp);
	inv_dt = 1.0f / dt;
}

void fluid_sim::set_v_bound(){
	const float w = 4.0f;

	// vx top and bottom
	for (uint i = 1; i < vx.dim_x()-1; i++){
		vx(i, 0) = -vx(i, 1);
		vx(i, vx.dim_y()-1) = -vx(i, vx.dim_y()-2);
	}
	// vx right and left
	for (uint j = 1; j < vx.dim_y()-1; j++){
		vx(0, j) = w;
		vx(vx.dim_x()-1, j) = vx(vx.dim_x()-2,j);
	}

	// vy top and bottom
	for (uint i = 1; i < vy.dim_x()-1; i++){
		vy(i, 0) = 0.0f;
		vy(i, vy.dim_y()-1) = 0.0f;
	}
	// vy right and left
	for (uint j = 1; j < vy.dim_y()-1; j++){
		vy(0, j) = - vy(1, j);
		vy(vy.dim_x()-1, j) = -vy(vy.dim_x()-2, j);
	}


	for (uint i = 200; i < 300; i++){
		for (uint j = 64; j < 193; j++){
			vx(i+sq(float(j)-128.0f)*0.05f,j) = 0.0f;
			vy(i+sq(float(j)-128.0f)*0.05f,j) = 0.0f;
		}
	}
}

void fluid_sim::compute_v_mag_max(){
	vx_mag_max = 0.0f;
	vy_mag_max = 0.0f;

	for (uint i = 0; i < vx.size(); i++){
		vx_mag_max = max(vx_mag_max, fabsf(vx[i]));
		vy_mag_max = max(vy_mag_max, fabsf(vy[i]));
	}
}

void fluid_sim::compute_poisson_rhs(){
	// compute vx_tmp
	for (uint i = 1; i < vx.dim_x()-1; i++){
		vx_tmp(i, 0) = vx(i, 0);
		vx_tmp(i, vx.dim_y()-1) = vx(i, vx.dim_y()-1);
	}

	for (uint j = 1; j < vx.dim_y()-1; j++){
		vx_tmp(0, j) = vx(0, j);
		vx_tmp(vx.dim_x()-1, j) = vx(vx.dim_x()-1, j);
	}


	for (uint j = 1; j < vx.dim_y()-1; j++){
		for (uint i = 1; i < vx.dim_x()-1; i++){

			const float tmp1 = (
				sq_inv_dx * ( vx(i+1,j) - 2.0f*vx(i,j) + vx(i-1,j) ) +
				sq_inv_dy * ( vx(i,j+1) - 2.0f*vx(i,j) + vx(i,j-1) ) 
			);

			const float tmp2 = 0.25f * inv_dx * (
				deriv_w * (
					sq( vx(i,j) + vx(i+1,j) ) -
					sq( vx(i-1,j) + vx(i,j) )
				) + 
				(1.0f - deriv_w) * (
					fabsf(vx(i,j) + vx(i+1,j)) * (vx(i,j) - vx(i+1,j)) -
					fabsf(vx(i-1,j) + vx(i,j)) * (vx(i-1,j) - vx(i,j)) 
				)
			);

			const float tmp3 = 0.25f * inv_dy * (
				deriv_w * (
					(vy(i,j) + vy(i+1,j)) * (vx(i,j) + vx(i,j+1)) -
					(vy(i,j-1) + vy(i+1,j-1)) * (vx(i,j-1) + vx(i,j))
				) + 
				(1.0f - deriv_w) * (
					fabsf(vy(i,j) + vy(i+1,j)) * (vx(i,j) - vx(i,j+1)) -
					fabsf(vy(i,j-1) + vy(i+1,j-1)) * (vx(i,j-1) - vx(i,j)) 
				)
			);

			vx_tmp(i,j) = vx(i,j) + dt * (inv_re * tmp1 - tmp2 - tmp3 + force.x);			
		}
	}

	// compute vy_tmp
	for (uint i = 1; i < vy.dim_x()-1; i++){
		vy_tmp(i, 0) = vy(i, 0);
		vy_tmp(i, vy.dim_y()-1) = vy(i, vy.dim_y()-1);
	}

	for (uint j = 1; j < vy.dim_y()-1; j++){
		vy_tmp(0, j) = vy(0, j);
		vy_tmp(vy.dim_x()-1, j) = vy(vy.dim_x()-1, j);
	}

	for (uint j = 1; j < vy.dim_y()-1; j++){
		for (uint i = 1; i < vy.dim_x()-1; i++){

			const float tmp1 = (
				sq_inv_dx * ( vy(i+1,j) - 2.0f*vy(i,j) + vy(i-1,j) ) +
				sq_inv_dy * ( vy(i,j+1) - 2.0f*vy(i,j) + vy(i,j-1) ) 
			);

			const float tmp2 = 0.25f * inv_dy * (
				deriv_w * (
					sq( vy(i,j) + vy(i,j+1) ) -
					sq( vy(i,j-1) + vy(i,j) )
				) + 
				(1.0f - deriv_w) * (
					fabsf(vy(i,j) + vy(i,j+1)) * (vy(i,j) - vy(i,j+1)) -
					fabsf(vy(i,j-1) + vy(i,j)) * (vy(i,j-1) - vy(i,j)) 
				)
			);

			const float tmp3 = 0.25f * inv_dx * (
				deriv_w * (
					(vy(i,j) + vy(i+1,j)) * (vx(i,j) + vx(i,j+1)) -
					(vx(i-1,j) + vx(i-1,j+1)) * (vy(i-1,j) + vy(i,j))
				) + 
				(1.0f - deriv_w) * (
					fabsf(vx(i,j) + vx(i,j+1)) * (vy(i,j) - vy(i+1,j)) -
					fabsf(vx(i-1,j) + vx(i-1,j+1)) * (vy(i-1,j) - vy(i,j)) 
				)

			);

			vy_tmp(i,j) = vy(i,j) + dt * (inv_re * tmp1 - tmp2 - tmp3 + force.y);
		}
	}

	// compute rhs
	for (uint j = 1; j < p.dim_y()-1; j++){
		for (uint i = 1; i < p.dim_x()-1; i++){
			rhs(i,j) = inv_dt * (
				inv_dx * ( vx_tmp(i,j) - vx_tmp(i-1,j) ) +
				inv_dy * ( vy_tmp(i,j) - vy_tmp(i,j-1) )
			);
		}
	}
}

void fluid_sim::compute_poisson_p_jacobi(){
	float res_l2_norm = INFINITY;
	float res_max_norm = INFINITY;
	uint n = 0;

	for	(; n < jacobi_it_max; n++){
		// vertices
		p_tmp(0,0) =(1.0f-jacobi_weight) * p(0,0) + 
		jacobi_weight * vert_const * (
			sq_inv_dx * p(1,0) +
			sq_inv_dy * p(0,1) -
			rhs(0,0)
		);

		p_tmp(p.dim_x()-1,0) =(1.0f-jacobi_weight) * p(p.dim_x()-1,0) + 
		jacobi_weight * vert_const * (
			sq_inv_dx * p(p.dim_x()-2,0) +
			sq_inv_dy * p(p.dim_x()-1,1) -
			rhs(p.dim_x()-1,0)
		);

		p_tmp(0,p.dim_y()-1) =(1.0f-jacobi_weight) * p(0,p.dim_y()-1) + 
		jacobi_weight * vert_const * (
			sq_inv_dx * p(1,p.dim_y()-1) +
			sq_inv_dy * p(0,p.dim_y()-2) -
			rhs(0,p.dim_y()-1)
		);

		p_tmp(p.dim_x()-1,p.dim_y()-1) =(1.0f-jacobi_weight) * p(p.dim_x()-1,p.dim_y()-1) + 
		jacobi_weight *	vert_const * (
			sq_inv_dx * p(p.dim_x()-2,p.dim_y()-1) +
			sq_inv_dy * p(p.dim_x()-1,p.dim_y()-2) -
			rhs(p.dim_x()-1,p.dim_y()-1)
		);


		// edge right and left
		for (uint j = 1; j < p.dim_y()-1; j++){
			p_tmp(0,j) = (1.0f-jacobi_weight) * p(0,j) + 
			jacobi_weight * edge_x_const * (
				sq_inv_dx * p(1,j) +
				sq_inv_dy * ( p(0,j+1) + p(0,j-1) ) -
				rhs(0,j)
			);

			p_tmp(p.dim_x()-1,j) = (1.0f-jacobi_weight) * p(p.dim_x()-1,j) + 
			jacobi_weight * edge_x_const * (
				sq_inv_dx * p(p.dim_x()-2,j) +
				sq_inv_dy * ( p(p.dim_x()-1,j+1) + p(p.dim_x()-1,j-1) ) -
				rhs(p.dim_x()-1,j)
			);
		}

		// edge top and bottom
		for (uint i = 1; i < p.dim_x()-1; i++){
			p_tmp(i,0) = (1.0f-jacobi_weight) * p(i,0) + 
			jacobi_weight *	edge_y_const * (
				sq_inv_dx * ( p(i+1,0) + p(i-1,0) ) +
				sq_inv_dy * p(i,1) -
				rhs(i,0)
			);

			p_tmp(i,p.dim_y()-1) = (1.0f-jacobi_weight) * p(i,p.dim_y()-1) + 
			jacobi_weight *	edge_y_const * (
				sq_inv_dx * ( p(i+1,p.dim_y()-1) + p(i-1,p.dim_y()-1) ) +
				sq_inv_dy * p(i,p.dim_y()-2) -
				rhs(i,p.dim_y()-1)
			);
		}

		// inner cells
		for (uint j = 1; j < p.dim_y()-1; j++){
			for (uint i = 1; i < p.dim_x()-1; i++){
				p_tmp(i,j) = (1.0f-jacobi_weight) * p(i,j) + 
				jacobi_weight * inner_const * (
					sq_inv_dx * (p(i+1,j) + p(i-1,j)) +
					sq_inv_dy * (p(i,j+1) + p(i,j-1)) -
					rhs(i,j)
				);
			}
		}

		swap_data(p, p_tmp);


		// rectify_p();

		// compute residual norm
		compute_res();
		res_l2_norm = 0.0f;
		res_max_norm = 0.0f;

		for (uint i = 0; i < res.size(); i++){
			res_l2_norm += xmath::op::sq(res[i]);
			res_max_norm = max(res_max_norm, fabsf(res[i]));
		}

		res_l2_norm = res_l2_norm_const * sqrtf(res_l2_norm);

		// if (res_l2_norm <= 50.0f)
		// 	break;
	}

	printf("jacobi:\tit: %u\tres: %f\n", n, res_max_norm);
}

void fluid_sim::compute_poisson_p_sor(){
	for	(uint it = 0; it < sor_it_max; it++){
		p(0,0) = (1.0f - sor_relax) * p(0,0) + 
			sor_relax * vert_const * (
				sq_inv_dx * p(1,0) +
				sq_inv_dy * p(0,1) -
				rhs(0,0)
			);

		for (uint i = 1; i < p.dim_x()-1; i++){
			p(i,0) = (1.0f - sor_relax) * p(i,0) +
				sor_relax * edge_y_const * (
					sq_inv_dx * ( p(i+1,0) + p(i-1,0) ) +
					sq_inv_dy * p(i,1) -
					rhs(i,0)
				);
		}

		p(p.dim_x()-1,0) = (1.0f - sor_relax) * p(p.dim_x()-1,0) +
			sor_relax * vert_const * (
				sq_inv_dx * p(p.dim_x()-2, 0) +
				sq_inv_dy * p(p.dim_x()-1, 1) -
				rhs(p.dim_x()-1,0)
			);

		for (uint j = 1; j < p.dim_y()-1; j++){ // field is stored in row-major order, so j has to be first

			p(0,j) = (1.0f - sor_relax) * p(0,j) +
				sor_relax * edge_x_const * (
					sq_inv_dx * p(1,j) +
					sq_inv_dy * ( p(0,j+1) + p(0,j-1) ) -
					rhs(0,j)
				);

			for (uint i = 1; i < p.dim_x()-1; i++){
				// p(i,j) will be overwritten after computation (advantage of sor)
				p(i,j) = (1.0f - sor_relax) * p(i,j) +
					sor_relax * inner_const * (
						sq_inv_dx * (p(i+1,j) + p(i-1,j)) +
						sq_inv_dy * (p(i,j+1) + p(i,j-1)) -
						rhs(i,j)
					);
			}

			p(p.dim_x()-1,j) = (1.0f - sor_relax) * p(p.dim_x()-1,j) +
				sor_relax * edge_x_const * (
					sq_inv_dx * p(p.dim_x()-2,j) +
					sq_inv_dy * ( p(p.dim_x()-1,j+1) + p(p.dim_x()-1,j-1) ) -
					rhs(p.dim_x()-1,j)
				);
		}

		p(0, p.dim_y()-1) = (1.0f - sor_relax) * p(0,p.dim_y()-1) +
			sor_relax * vert_const * (
				sq_inv_dx * p(1, p.dim_y()-1) +
				sq_inv_dy * p(0, p.dim_y()-2) -
				rhs(0, p.dim_y()-1)
			);

		for (uint i = 1; i < p.dim_x()-1; i++){			
			p(i,p.dim_y()-1) = (1.0f - sor_relax) * p(i,p.dim_y()-1) +
				sor_relax * edge_y_const * (
					sq_inv_dx * ( p(i+1,p.dim_y()-1) + p(i-1,p.dim_y()-1) ) +
					sq_inv_dy * p(i,p.dim_y()-2) -
					rhs(i,p.dim_y()-1)
				);
		}

		p(p.dim_x()-1, p.dim_y()-1) = (1.0f - sor_relax) * p(p.dim_x()-1, p.dim_y()-1) +
			sor_relax * vert_const * (
				sq_inv_dx * p(p.dim_x()-2, p.dim_y()-1) +
				sq_inv_dy * p(p.dim_x()-1, p.dim_y()-2) -
				rhs(p.dim_x()-1, p.dim_y()-1)
			);
	}
}

void fluid_sim::compute_res(){
	// // vertices
	// res(0,0) = 
	// 	sq_inv_dx * (p(1,0) - p(0,0)) +
	// 	sq_inv_dy * (p(0,1) - p(0,0)) -
	// 	rhs(0,0);

	// res(p.dim_x()-1,0) = 
	// 	sq_inv_dx * (p(p.dim_x()-2,0) - p(p.dim_x()-1,0)) +
	// 	sq_inv_dy * (p(p.dim_x()-1,1) - p(p.dim_x()-1,0)) -
	// 	rhs(p.dim_x()-1,0);

	// res(0,p.dim_y()-1) = 
	// 	sq_inv_dx * (p(1,p.dim_y()-1) - p(0,p.dim_y()-1)) +
	// 	sq_inv_dy * (p(0,p.dim_y()-2) - p(0,p.dim_y()-1)) -
	// 	rhs(0,p.dim_y()-1);

	// res(p.dim_x()-1,p.dim_y()-1) = 
	// 	sq_inv_dx * (p(p.dim_x()-2,p.dim_y()-1) - p(p.dim_x()-1,p.dim_y()-1)) +
	// 	sq_inv_dy * (p(p.dim_x()-1,p.dim_y()-2) - p(p.dim_x()-1,p.dim_y()-1)) -
	// 	rhs(p.dim_x()-1,p.dim_y()-1);

	// // edge right and left
	// for (uint j = 1; j < p.dim_y()-1; j++){
	// 	// left
	// 	res(0,j) = 
	// 		sq_inv_dx * (p(1,j) - p(0,j)) +
	// 		sq_inv_dy * (p(0,j+1) - p(0,j-1) - 2.0f*p(0,j)) -
	// 		rhs(0,j);
		
	// 	// right
	// 	res(p.dim_x()-1,j) = 
	// 		sq_inv_dx * (p(p.dim_x()-2,j) - p(p.dim_x()-1,j)) +
	// 		sq_inv_dy * (p(p.dim_x()-1,j+1) - p(p.dim_x()-1,j-1) - 2.0f*p(p.dim_x()-1,j)) -
	// 		rhs(p.dim_x()-1,j);
	// }

	// // edge top and bottom
	// for (uint i = 1; i < p.dim_x()-1; i++){
	// 	// down
	// 	res(i,0) = 
	// 		sq_inv_dx * (p(i+1,0) - p(i-1,0) - 2.0f*p(i,0)) +
	// 		sq_inv_dy * (p(i,1) - p(i,0)) -
	// 		rhs(i,0);
		
	// 	// up
	// 	res(i,p.dim_y()-1) = 
	// 		sq_inv_dx * (p(i+1,p.dim_y()-1) - p(i-1,p.dim_y()-1) - 2.0f*p(i-1,p.dim_y()-1)) +
	// 		sq_inv_dy * (p(i,p.dim_y()-2) - p(i,p.dim_y()-1)) -
	// 		rhs(i,p.dim_y()-1);
	// }

	// inner cells
	for (uint j = 1; j < p.dim_y()-1; j++){
		for (uint i = 1; i < p.dim_x()-1; i++){
			res(i,j) =
				sq_inv_dx * (p(i+1,j) + p(i-1,j) - 2.0f*p(i,j)) +
				sq_inv_dy * (p(i,j+1) + p(i,j-1) - 2.0f*p(i,j)) -
				rhs(i,j);
		}
	}

	// compute residual norm
	// res_l2_norm = 0.0f;
	// res_max_norm = 0.0f;

	// for (uint i = 0; i < res.size(); i++){
	// 	res_l2_norm += xmath::op::sq(res[i]);
	// 	res_max_norm = max(res_max_norm, fabsf(res[i]));
	// }

	// res_l2_norm = res_l2_norm_const * sqrtf(res_l2_norm);
}

void fluid_sim::rectify_p(){
	// correct mean of p to zero
	float p_err = 0.0f;

	for (uint i = 0; i < p.size(); i++){
		p_err += p[i];
	}

	p_err *= 1.0f / float(p.size());

	for (uint i = 0; i < p.size(); i++){
		p[i] -= p_err;
	}
}

void fluid_sim::compute_v(){
	for (uint j = 1; j < vx.dim_y()-1; j++){
		for (uint i = 1; i < vx.dim_x()-1; i++){
			vx(i,j) = vx_tmp(i,j) - dt * inv_dx * (p(i+1,j) - p(i,j));
		}
	}

	for (uint j = 1; j < vy.dim_y()-1; j++){
		for (uint i = 1; i < vy.dim_x()-1; i++){
			vy(i,j) = vy_tmp(i,j) - dt * inv_dy * (p(i,j+1) - p(i,j));
		}
	}
}

void fluid_sim::compute(){
	compute_dt();
	set_v_bound();
	compute_poisson_rhs();
	compute_poisson_p_jacobi();
	// compute_poisson_p_sor();
	rectify_p();
	compute_v();
	compute_v_mag_max();
	// set_v_bound();

	t += dt;
	it++;
}
