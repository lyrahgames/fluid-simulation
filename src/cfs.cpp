// #include <cfs.h>

// inline void CFS::compute_equ(){
// 	for (sint i = 1; i < _grid[0].size() - 1; i++){
// 		for (sint j = 1; j < _grid[1].size() - 1; j++){
// 			const sint idx = _grid[0].size() * i + j;



// 			// laplace operator v[0]
// 			const real tmp11 = 




// 			const float F_tmp1 = Re_inv * ( (u(i+1,j) - 2*u(i,j) + u(i-1,j))/(grid.scale().x * grid.scale().x) + (u(i,j+1) - 2*u(i,j) + u(i,j-1))/(grid.scale().y * grid.scale().y) );

// 			const float F_tmp2 = grid.invScale().x * ( sq( 0.5f * (u(i,j) + u(i+1,j)) ) - sq( 0.5f * (u(i-1,j) + u(i,j)) ) )
// 								+ gamma * grid.invScale().x * 0.25f * ( fabs(u(i,j) + u(i+1,j)) * (u(i,j) - u(i+1,j))  -  fabs(u(i-1,j) + u(i,j)) * (u(i-1,j) - u(i,j)) );

// 			const float F_tmp3 = grid.invScale().y * 0.25f * ( (v(i,j) + v(i+1,j))*(u(i,j) + u(i,j+1)) - (v(i,j-1) + v(i+1,j-1))*(u(i,j-1) + u(i,j)) )
// 								+ gamma * grid.invScale().y * 0.25f * ( fabs(v(i,j) + v(i+1,j))*(u(i,j) - u(i,j+1)) - fabs(v(i,j-1) + v(i+1,j-1))*(u(i,j-1) - u(i,j)) );

// 			F(i,j) = u(i,j) + dt * ( F_tmp1 - F_tmp2 - F_tmp3 + g.x );


// 			const float G_tmp1 = Re_inv * ( (v(i+1,j) - 2*v(i,j) + v(i-1,j))/(grid.scale().x * grid.scale().x) + (v(i,j+1) - 2*v(i,j) + v(i,j-1))/(grid.scale().y * grid.scale().y) );
// 			const float G_tmp2 = grid.invScale().y * ( sq( 0.5f * (v(i,j) + v(i,j+1)) ) - sq( 0.5f * (v(i,j-1) + v(i,j)) ) )
// 								+ gamma * grid.invScale().y * 0.25f * ( fabs(v(i,j) + v(i,j+1)) * (v(i,j) - v(i,j+1))  -  fabs(v(i,j-1) + v(i,j)) * (v(i,j-1) - v(i,j)) );
// 			const float G_tmp3 = grid.invScale().x * 0.25f * ( (v(i,j) + v(i+1,j))*(u(i,j) + u(i,j+1)) - (u(i-1,j) + u(i-1,j+1))*(v(i-1,j) + v(i,j)) )
// 								+ gamma * grid.invScale().x * 0.25f * ( fabs(u(i,j) + u(i,j+1))*(v(i,j) - v(i+1,j)) - fabs(u(i-1,j) + u(i-1,j+1))*(v(i-1,j) - v(i,j)) );
// 			G(i,j) = v(i,j) + dt * ( G_tmp1 - G_tmp2 - G_tmp3 + g.y );

// 			RHS(i,j) = inv_dt * ( grid.invScale().x * (F(i,j) - F(i-1,j)) + grid.invScale().y * (G(i,j) - G(i,j-1)) );
// 		}
// 	}
// }