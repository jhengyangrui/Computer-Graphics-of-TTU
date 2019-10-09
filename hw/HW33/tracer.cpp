#include <gl/glut.h>
#include <stdio.h>

// transform screen (x,y) to a point on XZ plane (world)
void screen2object (int x, int y, float* xz)
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;  /*  OpenGL y coordinate position  */
	GLdouble wx0,wy0,wz0, wx1,wy1,wz1;  /*  returned world x, y, z coords  */
    GLdouble t ;

	glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
/*  note viewport[3] is height of window in pixels  */
    realy = viewport[3] - (GLint) y - 1;
		
//	printf ("Coordinates at cursor are (%4d, %4d)\n", x, realy);
    gluUnProject ((GLdouble) x, (GLdouble) realy, 0.0, 
            mvmatrix, projmatrix, viewport, &wx0, &wy0, &wz0);
//    printf ("World coords at z=0.0 are (%f, %f, %f)\n", wx0, wy0, wz0);
    gluUnProject ((GLdouble) x, (GLdouble) realy, 1.0, 
            mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1); 
//    printf ("World coords at z=1.0 are (%f, %f, %f)\n", wx1, wy1, wz1);

	// find intersection at XZ plane (y=0)
    t = wy0/(wy0-wy1);
	xz[0] = (1-t)*wx0 + t*wx1;
	xz[2] = (1-t)*wz0 + t*wz1;
    xz[1] = 0.0;
#if 0
	float depth[1];
	glReadPixels(x, realy, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
	printf ("%f\n", depth[0]);
    gluUnProject ((GLdouble) x, (GLdouble) realy, depth[0], 
            mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1); 
    printf ("World coords at mouse are (%f, %f, %f)\n", wx1, wy1, wz1);
#endif
}

