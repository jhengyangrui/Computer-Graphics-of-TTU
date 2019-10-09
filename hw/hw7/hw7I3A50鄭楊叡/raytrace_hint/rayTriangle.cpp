
//
// paper: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
// url: http://jgt.akpeters.com/papers/MollerTrumbore97/code.html
// 
#include <stdio.h>
#define Float double 

#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int  // return 0 (no intersection) 1 (otherwise)
intersect_triangle(Float orig[3], Float dir[3],
                   Float vert0[3], Float vert1[3], Float vert2[3],
                   Float *t, Float *u, Float *v)
{
   Float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   Float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
   if (det < EPSILON)
      return 0;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec);
   if (*u < 0.0 || *u > det)
      return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec);
   if (*v < 0.0 || *u + *v > det)
      return 0;

   /* calculate t, scale parameters, ray intersects triangle */
   *t = DOT(edge2, qvec);
   inv_det = 1.0 / det;
   *t *= inv_det;
   *u *= inv_det;
   *v *= inv_det;
#else                    /* the non-culling branch */
   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0 / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
#endif
   return 1;  // only test [u,v] range. t can be *any*
}


#define SetVec(x,y,z,v) \
	v[0] = x; \
	v[1] = y; \
	v[2] = z;
#if 0
main ()
{
	Float t, u, v;
	Float orig[3], dir[3], v0[3], v1[3], v2[3];

	SetVec (0.82, 0.82, 4.0, orig);
	SetVec (0.0, 0.0, -1.0, dir);

	SetVec (0.0, 0.0, 0.0, v0);
	SetVec (1.0, 0.0, 0.0, v1);
	SetVec (0.0, 1.0, 0.0, v2);

	t = 0.0;
	int result = intersect_triangle (orig, dir, v0, v1, v2, &t, &u, &v);

	printf ("[%d] %f %f %f\n", result, t, u, v);
}
#endif