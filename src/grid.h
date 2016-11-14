#ifndef __GRID_H__
#define __GRID_H__

#include <util.h>


template <class T, class N>
struct GridMap{
	// types used for definition of template
	using real_type = T;
	using size_type = N;


	public:
		GridMap(): _min(0), _max(1), _size(1), _step(1), _inv_step(1){}
		GridMap(T in_min, T in_max, N in_cell_count): GridMap(){
			set_intvl(in_min, in_max);
			set_cell_count(in_cell_count);
		}


		// minimum of interval
		T min() const{return _min;}
		// maximum of interval
		T max() const{return _max;}
		//
		T length() const{return _max - _min;}
		// size of the GridMap = cell count
		N size() const{return _size;}
		// get cell count
		N cell_count() const{return size();}
		// get node count
		N node_count() const{return size()+1;}
		// get step (distance between two nodes)
		T step() const{return _step;}
		// inverse of step
		T inv_step() const{return _inv_step;}

		// get position to given node (overloaded to avoid casting)
		// T node_pos(N idx) const{return min() + step()*static_cast<T>(idx);}
		T node_pos(T idx) const{return min() + step()*idx;}
		// get position to given cell (overloaded to avoid casting)
		// T cell_pos(N idx) const{return node_pos(idx) + 0.5*step;}
		T cell_pos(T idx) const{return node_pos(idx) + 0.5*step();}

		// get exact node index for given point (no rounding or casting)
		T node_idx(T in_pos) const{return (in_pos - min()) * inv_step();}
		// get cell index
		T cell_idx(T in_pos) const{return node_idx(in_pos) - 0.5;}


		// set size/cell count of GridMap
		void set_size(N in_size);
		// set cell count	
		void set_cell_count(N in_count){set_size(in_count);}
		// set node count of GridMap
		void set_node_count(N in_node_count){set_size(in_node_count - 1);}
		// set interval for transformation
		void set_intvl(T in_min, T in_max);


	private:
		// precompute step values
		void compute_step();


	private:
		// intelval parameter
		T _min, _max;
		// size/cell count
		N _size;
		// temporary step values for transformation
		T _step, _inv_step;
};


template <class T, class N>
inline void GridMap<T,N>::set_size(N in_size){
	if (in_size <= 0)
		return;

	_size = in_size;
	compute_step();
}

template <class T, class N>
inline void GridMap<T,N>::set_intvl(T in_min, T in_max){
	if (in_min >= in_max)
		return;

	_min = in_min;
	_max = in_max;
	compute_step();
}

template <class T, class N>
inline void GridMap<T,N>::compute_step(){
	_step = (_max - _min) / static_cast<T>(_size);
	_inv_step = static_cast<T>(_size) / (_max - _min);
}


using gridmap = GridMap<float,uint>;


#endif // __GRID_H__