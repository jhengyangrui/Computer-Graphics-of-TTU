#include <svl/svl.h>

// solve ax^2 + bx + c = 0
// return 0 if no solution; 1 otherwise
static int solve (double abc[3], double &x1, double &x2)
{
	double det = abc[1]*abc[1] - 4*abc[0]*abc[2];

	if (det < 0) 
		return 0;

	// singular: double root
	//det = sqrt(det);
	//*x1 = (-b-det)/2/a;
	//*x2 = (-b+det)/2/a;

	// from http://mathworld.wolfram.com/QuadraticEquation.html
	double q;

	#define Sgn(x) ((x)>0 ? 1 : -1)
	q = -0.5*(abc[1]+Sgn(abc[1])*det);
	x1 = q/abc[0]; x2 = abc[2]/q;

	return 1;
}


// model
// ellipsoid: (a,b,c) = (2,1,1)
// 
static const double AA = 2.0;
static const double BB = 1.0;
static const double CC = 1.0;

bool ray_ellipsoid (const Vec3 &c, const Vec3 &r, double &t)
{
	Vec3 rs(r[0]/AA, r[1]/BB, r[2]/CC);
	Vec3 cs(c[0]/AA, c[1]/BB, c[2]/CC);

	double t1, t2;
	int ans;
	double abc[] = {dot(rs,rs), 2*dot(rs,cs), dot(cs,cs)-1};
	ans = solve (abc, t1, t2);

	t = 0.0;
	if (! ans)
		return false;
	
	t = Min (t1, t2);
	if (t > 0) 
		return true;  // should only take positive solution!!
	else
		return false;
}


void EllipsoidNormal (const Vec3 &pos, Vec3& normal)
{
#define Sqr(x) ((x)*(x))
	normal[0] = pos[0]/Sqr(AA);
	normal[1] = pos[1]/Sqr(BB);
	normal[2] = pos[2]/Sqr(CC);

	normalise (normal);
}

