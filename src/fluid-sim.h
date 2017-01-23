#ifndef __FLUID_SIM_H__
#define __FLUID_SIM_H__

#include <util.h>


struct fluid_sim{
	public:
		// fluid_sim() = default;
		fluid_sim(uint width, uint height, const intvlv& geometry);
		// fluid_sim(const vecu2& dimension, const intvlv& geometry);


	public:
		void init();

		// set geometry of box
		void set_geometry(const intvlv& geometry);
		// set grid dimension
		void set_grid_dim(const vecu2& grid_dim);
		void set_grid_dim(uint width, uint height);


		void set_reynold(float reynold){ re = reynold; inv_re = 1.0f/re; compute_dt_bound(); }


		// clear every array
		void clear();

		// precompute grid constants
		void compute_grid_const();
		// compute upper boundary for time step
		void compute_dt_bound();
		// compute time step
		void compute_dt();
		//
		void set_v_bound();
		//
		void compute_poisson_rhs();
		// compute jacobi algorithm for poisson equation for pressure
		void compute_poisson_p_jacobi();
		//
		void compute_poisson_p_sor();
		// compute residuum
		void compute_res();
		// rectify pressure for preventing divergence
		void rectify_p();
		// compute velocity out of pressure
		void compute_v();
		// compute next iteration
		void compute();


	public:
		intvlv geom; // box geometry
		vecu2 dim; // grid size/dimension
		fieldf p; // pressure
		fieldf p_tmp; // temporary pressure field
		fieldf rhs; // right-handside of poisson pressure equation
		fieldf res; // residuum
		float res_eps;
		fieldf vx; // velocity in x direction
		fieldf vx_tmp;
		fieldf vy; // velocity in y direction
		fieldf vy_tmp;
		float vx_max;
		float vx_min;
		float vy_max;
		float vy_min;

		float t; // time
		uint it; // current iteration

		float dt_safe; // time step safety factor
		float dt_bound; // upper bound for time step
		float dt; // time step
		float inv_dt; // inverse time step

		float dx; // x step
		float dy; // y step

		float re; // reynold number
		float inv_re; // inverse reynold number

		vecf2 force;

		float deriv_w; // weight of normal derivative

		uint jacobi_it_max; // maximum of iterations in jacobi algorithm
		float jacobi_weight;
		uint sor_it_max; // maximum of iterations in sor algorithm
		float sor_relax; // relaxation constant of sor algorithm


	public:
		float inv_dx;
		float inv_dy;
		float sq_inv_dx;
		float sq_inv_dy;
		float inner_const;
		float edge_x_const;
		float edge_y_const;
		float vert_const;
		float res_l2_norm_const;
};


#endif // __FLUID_SIM_H__