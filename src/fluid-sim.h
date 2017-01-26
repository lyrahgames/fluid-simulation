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


		const intvlv& geometry() const{return geom;}
		float time() const{return t;}
		float time_step() const{return dt;}
		float time_step_safe() const{return dt_safe;}
		float time_step_bound() const{return dt_bound;}
		float max_speed_x() const{return vx_mag_max;}
		float max_speed_y() const{return vy_mag_max;}
		float max_speed() const{return sqrtf(sq( max_speed_x()) + sq(max_speed_y()) );}
		float reynold() const{return re;}
		float border_speed() const{return border_v;}
		const vecf2& force() const{return f;}

		const vecu2& grid_dim() const{return dim;}
		float x_step() const{return dx;}
		float y_step() const{return dy;}
		uint iteration() const{return it;}
		float deriv_weight() const{return deriv_w;}
		uint jacobi_max_iteration() const{return jacobi_it_max;}
		float jacobi_weight() const{return jacobi_w;}


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
		void compute_v_mag_max();
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
		fieldf obs;
		fieldf vx; // velocity in x direction
		fieldf vx_tmp;
		fieldf vy; // velocity in y direction
		fieldf vy_tmp;
		float vx_max;
		float vx_min;
		float vx_mag_max;
		float vy_max;
		float vy_min;
		float vy_mag_max;
		float border_v;

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

		vecf2 f; // force

		float deriv_w; // weight of normal derivative

		uint jacobi_it_max; // maximum of iterations in jacobi algorithm
		float jacobi_w; // weight
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


		uint down_boundx;
		uint down_boundy;
		uint up_boundx;
		uint up_boundy;
		uint left_boundx;
		uint left_boundy;
		uint right_boundx;
		uint right_boundy;
		uint obs_number;


	public:
		vecu2 user_idx;
		vecf2 user_v;
};


#endif // __FLUID_SIM_H__