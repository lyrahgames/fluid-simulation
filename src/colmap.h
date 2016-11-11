#ifndef __COLMAP_H__
#define __COLMAP_H__

#include <util.h>


struct fcolrgb{
	float r,g,b;
};


template<uint N>
struct colmap{
	float sample_pos[N];
	fcolrgb sample_col[N];

	
};


#endif // __COLMAP_H__