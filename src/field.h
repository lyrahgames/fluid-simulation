#ifndef __FIELD_H__
#define __FIELD_H__

#include <string.h>

#include <util.h>
#include <grid.h>


struct field{
	public:
		field(const gridmap& gridmapx, const gridmapy): gmx(gridmap), gmy(gridmap), data(nullptr){
			init();
		}
		~field(){delete[] data;}



		float& operator[](uint idx) { return data[idx]; }
		const float& operator[](uint idx) const { return data[idx]; }

		const gridmap& gmx() const{return gmx;}
		const gridmap& gmy() const{return gmy;}

		float& operator()(uint i, uint j){return data[j*gmx.node_count() + i];}
		const float& operator()(uint i, uint j) const{return data[j*gmx.node_count() + i];}

		void set(float v){
			for (uint i = 0; i < gmx.node_count() * gmy.node_count(); i++)
				data[i] = v;
		}

	private:
		float *data;
		gridmap gmx, gmy;


	private:
		void init(){
			if (data != nullptr)
				delete[] data;

			const uint size = gmx.node_count() * gmy.node_count();
			data = new float[size];
		}
};

using fieldf = field<float>;


inline float operator()(const field& f, float x, float y){
	const float mx = f.gmx().node_idx(x);
	const float my = f.gmy().node_idx(y);
	const float mi = floor(mx);
	const float mj = floor(my);
	const uint i = static_cast<uint>(mi);
	const uint j = static_cast<uint>(mj);
	const float wi = mx - mi;
	const float wj = my - mj;
	// const uint idx1 = p_memidx(idx_x, idx_y);
	// const uint idx2 = p_memidx(idx_x, idx_y+1);

	return bilin_interp(f(i,j), f(i+1,j), f(i,j+1), f(i+1,j+1), wi, wj);
}


#endif // __FIELD_H__