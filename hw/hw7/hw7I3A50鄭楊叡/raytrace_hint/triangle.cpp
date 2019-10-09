#include <svl/svl.h>

extern int intersect_triangle(  // rayTriangle.cpp
double[], double[], double[],double[],double[], // origin, dir, v0,v1,v2
double*, double*, double*);   // t,u,v
		
bool ray_triangle (const Vec3 &point, const Vec3 &dir, 
				const Vec3 &v0,const Vec3 &v1,const Vec3 &v2, double &tt)
{
	double t, u, v;
	int result = intersect_triangle (point.Ref(), dir.Ref(), 
		v0.Ref(),v1.Ref(),v2.Ref(), &t,&u,&v );
	
	
	if (result && t >= 0) {
		tt = t;
		return true;
	} else
		return false;			
}
