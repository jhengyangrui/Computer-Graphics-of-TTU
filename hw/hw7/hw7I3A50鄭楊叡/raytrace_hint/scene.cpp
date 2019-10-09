#include <svl/svl.h>

extern bool ray_ellipsoid (const Vec3&, const Vec3&, double&);  // quadrics.cpp
extern bool ray_triangle (const Vec3&, const Vec3&, const Vec3&,const Vec3&,const Vec3&,double&); // triangle.cpp

extern void EllipsoidNormal (const Vec3&, Vec3&);  // quadrics.cpp

// ray: defined by (point, dir)
// intersect with scene_object (object_id)
// returns TRUE or FALSE
// with ray length (t >= 0) if TRUE
// intersect driver ...
bool intersect (const Vec3 &point, const Vec3 &dir, double &t, const int object_id)
{
	// scene description (1: ellipsoid; 2 & 3: plane)
	Vec3 v00 (-4,-2,4);
	Vec3 v01 (-4,-2,-4);
	Vec3 v10 (4,-2,4);
	Vec3 v11 (4,-2,-4);
	
	t = 0.0;
	switch (object_id) {
		case 1:
			return ray_ellipsoid (point, dir, t);
		case 2:
			return ray_triangle (point, dir, v00, v10, v01, t);
		case 3:
			return ray_triangle (point, dir, v10, v11, v01, t);
		default:
			return false;
	}
}

void ObjectNormal (const Vec3 &point, const int object_id, Vec3 &normal)
{
	switch (object_id) {
	case 1:
		EllipsoidNormal(point, normal);
		break;
	case 2:
	case 3:
		normal[0] = 0, normal[1] = 1.0, normal[2] = 0;  // normal plane always +Y
		break;
	}
}

struct _material {
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	int shininess;
};

void ObjectMaterial (const int object_id, struct _material *mat)
{
	switch (object_id) {
	case 1:  // ellipsoid
		mat->ambient = Vec3 (.2,.2,.2);
		mat->diffuse = Vec3 (.7,.7,.7);
		mat->specular = Vec3 (0.7,0,0);
		mat->shininess = 100;
		break;
	case 2:
		mat->ambient = Vec3 (.2,.2,.2);
		mat->diffuse = Vec3 (0.5,0.8,0.0);
		mat->specular = Vec3 (0,0.7,0);
		mat->shininess = 100;
		break;
	case 3:
		mat->ambient = Vec3 (.2,.2,.2);
		mat->diffuse = Vec3 (0.0,0.8,0.5);
		mat->specular = Vec3 (0,0.7,0);
		mat->shininess = 100;
		break;
	}
	
}
