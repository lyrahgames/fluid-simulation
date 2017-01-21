#include <fluid-sim.h>


fluid_sim::fluid_sim(uint width, uint height, const intvlv& geometry):
geom(geometry), dim(vecu2{width, height}),
p(fieldf(width,height)), p_tmp(fieldf(width,height)), rhs(fieldf(width,height)),
vx(fieldf(width-1,height)), vx_tmp(fieldf(width-1,height)),
vy(fieldf(width,height-1)), vy_tmp(fieldf(width,height-1)),
vx_max(0.0f), vx_min(0.0f), vy_max(0.0f), vy_min(0.0f),
t(0.0f), it(0),
dt_safe(0.5f),
re(1000.0f),
force(vecf2()),
deriv_w(0.5f),
jacobi_it_max(100){
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
	sq_inv_dx = dx * dx;
	sq_inv_dy = dy * dy;
	inner_const = 0.5f / (sq_inv_dx + sq_inv_dy);
	edge_x_const = 1.0f / (sq_inv_dx + 2.0f*sq_inv_dy);
	edge_y_const = 1.0f / (2.0f*sq_inv_dx + sq_inv_dy);
	vert_const = 2.0f * inner_const;
}

void fluid_sim::compute_dt_bound(){
	dt_bound = dt_safe * re * inner_const;
}

void fluid_sim::compute_dt(){
	const float vx_abs_max = max(fabsf(vx_max), fabsf(vx_min));
	const float vy_abs_max = max(fabsf(vy_max), fabsf(vy_min));

	const float tmp = min(dx/vx_abs_max, dy/vy_abs_max);
	dt = min(dt_bound, dt_safe*tmp);
	inv_dt = 1.0f / dt;
}

void fluid_sim::set_v_bound(){
	const float w = 0.5f;

	// top and bottom
	for (uint i = 1; i < vx.dim_x()-1; i++){
		vy(i, 0) = 0.0f;
		vy(i, vy.dim_y()-1) = 0.0f;

		vx(i, 0) = -vx(i, 1);
		vx(i, vx.dim_y()-1) = -vx(i, vx.dim_y()-2);
	}

	vy(vy.dim_x()-1, 0) = 0.0f;
	vy(vy.dim_x()-1, vy.dim_y()-1) = 0.0f;


	// right and left
	for (uint j = 1; j < vy.dim_y()-1; j++){
		vx(0, j) = w;
		vx(vx.dim_x()-1, j) = w;

		vy(0, j) = -vy(1, j);
		vy(vy.dim_x()-1, j) = -vy(vy.dim_x()-2, j);
	}

	vx(0, vx.dim_y()-1) = w;
	vx(vx.dim_x()-1, vx.dim_y()-1) = w;


	vx_max = w;
	vy_max = 0.0f;
}

void fluid_sim::compute_poisson_rhs(){
	// compute vx_tmp
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
	for	(uint it = 0; it < jacobi_it_max; it++){
		// edge right and left
		for (uint j = 1; j < p.dim_y()-1; j++){
			// left
			p_tmp(0,j) = 
			edge_x_const * (
				sq_inv_dx * p(1,j) +
				sq_inv_dy * ( p(0,j+1) + p(0,j-1) ) -
				rhs(0,j)
			);
			
			// right
			p_tmp(p.dim_x()-1,j) = 
			edge_x_const * (
				sq_inv_dx * p(p.dim_x()-2,j) +
				sq_inv_dy * ( p(p.dim_x()-1,j+1) + p(p.dim_x()-1,j-1) ) -
				rhs(p.dim_x()-1,j)
			);
		}

		// edge top and bottom
		for (uint i = 1; i < p.dim_x()-1; i++){
			// down
			p_tmp(i,0) = 
			edge_y_const * (
				sq_inv_dx * ( p(i+1,0) + p(i-1,0) ) +
				sq_inv_dy * p(i,1) -
				rhs(i,0)
			);
			
			// up
			p_tmp(i,p.dim_y()-1) = 
			edge_y_const * (
				sq_inv_dx * ( p(i+1,p.dim_y()-1) + p(i-1,p.dim_y()-1) ) +
				sq_inv_dy * p(i,p.dim_y()-2) -
				rhs(i,p.dim_y()-1)
			);
		}

		// inner cells
		for (uint j = 1; j < p.dim_y()-1; j++){
			for (uint i = 1; i < p.dim_x()-1; i++){
				p_tmp(i,j) = inner_const * (
					sq_inv_dx * (p(i+1,j) + p(i-1,j)) +
					sq_inv_dy * (p(i,j+1) + p(i,j-1)) -
					rhs(i,j)
				);
			}
		}

		swap_data(p, p_tmp);
	}
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
	compute_poisson_rhs();
	compute_poisson_p_jacobi();
	rectify_p();
	compute_v();
	set_v_bound();

	t += dt;
	it++;
}