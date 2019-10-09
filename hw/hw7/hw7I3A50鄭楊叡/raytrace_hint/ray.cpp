#include <gl/glut.h>
#include <SVL/SVL.h>

GLubyte image[256][256][3];

enum {R,G,B};


static void GetCameraFrame (Vec3& c, Vec3& point, Vec3& up)
{
	double m[16];
	glGetDoublev (GL_MODELVIEW_MATRIX, m);

	// explicit formulation: does not speed up as much as I expected ..
	c[0] = -m[0]*m[12]-m[1]*m[13]-m[2]*m[14];
	c[1] = -m[4]*m[12]-m[5]*m[13]-m[6]*m[14];
	c[2] = -m[8]*m[12]-m[9]*m[13]-m[10]*m[14];

	point[0] = -m[0]*m[12]-m[1]*m[13]-m[2]*(m[14]+1);
	point[1] = -m[4]*m[12]-m[5]*m[13]-m[6]*(m[14]+1);
	point[2] = -m[8]*m[12]-m[9]*m[13]-m[10]*(m[14]+1);
	point -= c; 

	// up: [0,1,0]
	up[0] = -m[0]*m[12]-m[1]*(m[13]-1)-m[2]*m[14];
	up[1] = -m[4]*m[12]-m[5]*(m[13]-1)-m[6]*m[14];
	up[2] = -m[8]*m[12]-m[9]*(m[13]-1)-m[10]*m[14];
	up -= c;
}

#define SetRGB(i,j,rgb) {\
    image[i][j][R] = rgb[R]*255;\
    image[i][j][G] = rgb[G]*255;\
    image[i][j][B] = rgb[B]*255;\
}

void DoRayTrace (void)
{
	int i, j;

	extern Vec3	raytrace (const Vec3&, const Vec3&, int&);
	extern Vec3 camera;
	extern Vec3 clamp (const Vec3&, const Vec3&, const Vec3&);
	
	// clear canvas
	memset (image, 0, 256*256*3);

	// get camera position
	Vec3 c, point, up;
	GetCameraFrame (c, point, up);
	
	camera = c;

	// op: origin of projection plane (lower left corner)
	const double glnear = 0.1;//1.0;
	const double fovy = 65.0;
	Vec3 op;
	op = c + glnear*point;
	double w = 2*glnear*tan(fovy/2/180*vl_pi);
	Vec3 V = up;
	Vec3 U = cross (point,V);
	op = op - w/2*U - w/2*V;

	Vec3 r, gridpt;
	Vec3 p, normal;
	double div = w/256;

	for (i = 0; i < 256; i++) {
		for (j = 0; j < 256; j++) {
	
			gridpt = op + (i+0.5)*div*V + (j+0.5)*div*U;
			r = norm (gridpt - c);
			int level = 1; // level 1 for eye ray
			Vec3 intensity = raytrace (c, r, level);  
			intensity = clamp (intensity, vl_0, vl_one);
			SetRGB (i,j,intensity);
		}
	}
	
}
