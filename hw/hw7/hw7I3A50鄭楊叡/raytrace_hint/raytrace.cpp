#include <svl/svl.h>
#include <stdio.h>

extern bool intersect (const Vec3&, const Vec3&, double&, int);
extern void ObjectNormal (const Vec3&, const int, Vec3&);
extern void ObjectMaterial (const int, struct _material*);

// check whether any scene object is between point (on baseobject) and light
bool ShadowFeeler (const Vec3 &point, const Vec3 &light, const int baseobject)
{
	// make an unit vector from point to light
	// get its distance

	// for all OTHER objects, do an "intersect" (if true, check its distance)
	// for blockage
	Vec3 dit = norm (light - point);
	double distance = len (light - point);

	extern int numobjects;
	for (int i = 1 ; i <= numobjects; i++) {
		double dd;
		if (i != baseobject && intersect (point, dit, dd, i) && dd < distance) {
			return true;
		}
	}
	return false; //(nothing in the way to light)	
}

// for the sake of efficiency
#define _clamp(v,min,max) ((v) > (max) ? (max) : ((v) < (min) ? (min) : (v)))

#if 0  // the following wont work ... 
#define clamp(v, min, max) Vec3(_clamp(v[0],min[0],max[0]),_clamp(v[1],min[1],max[1]),_clamp(v[2],min[2],max[2]));
#else
Vec3 clamp (const Vec3 &value, const Vec3 &min, const Vec3 &max)
{
	return Vec3(_clamp (value[0], min[0], max[0]),
				_clamp (value[1], min[1], max[1]),
				_clamp (value[2], min[2], max[2]));
}
#endif

// local illumination of objectid at point
// ambient, diffuse, specular
// also return reflect ray for later use
struct _material {
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	int shininess;
};

Vec3 EvaluateIlocal (const Vec3 &point, const int objectid, const Vec3 &n)
{
	extern Vec3 lightpos[], camera;
	extern int numlights;

	struct _material mat;
	ObjectMaterial (objectid, &mat);

	Vec3 intensity = vl_0;

	// I = Ilocal + Kr*R + Kt*T
	// assume one light ...

	for (int i = 0; i < numlights; i++) {
		intensity += mat.ambient; 
		Vec3 l = norm (lightpos[i] - point);

		double ndotl = dot(n, l);
		// front facing && no block
		if (ndotl > 0.0 && ShadowFeeler (point, lightpos[i], objectid) == false) { 
			intensity += ndotl * mat.diffuse;
			//
			// ... add (blinn-phong) specular component
			//
		}
	}
	return intensity;
}

// replace {find first hit}
// with a first-hit guess procedure using OpenGL prerender
// 
Vec3 raytrace (const Vec3 &c, const Vec3 &dir, int &level)
{	
	extern int maxlevel, numobjects;
//	if (level > 1) printf ("enter reflection ...\n");
	
	if (level > maxlevel) 
		return vl_0;
	
//	level++;

	// {find first hit (object, point)}
	int first_hit = 0;
	double min_d = 1e10;

	for (int i = 0; i < numobjects; i++) {
		double t;
		if (intersect (c, dir, t, i+1) && t < min_d) {
			first_hit = i+1; 
			min_d = t;
		}
	}
	
	
	if (first_hit) {
		Vec3 point = c + dir*min_d; // compute first hit point
		Vec3 normal;
		ObjectNormal (point, first_hit, normal);
		Vec3 intensity = EvaluateIlocal (point, first_hit, normal);
		double Kr = 0.2;
		extern Vec3 camera;

		Vec3 reflect = norm (2*dot(camera-point, normal)*normal - (camera-point));

		intensity += Kr * clamp(raytrace(point, reflect, ++level),vl_0,vl_1);

	//    if (transparent) 
	// 		 {trace the ray in the object, until it move out}
	//         (consider total reflection)
	//       intensity += Kt * raytrace(outpoint, outdir, level+1)	
		return intensity;

	} else { // nothing hit
		return vl_0; //Vec3(0,0,0); // background color
	}

}