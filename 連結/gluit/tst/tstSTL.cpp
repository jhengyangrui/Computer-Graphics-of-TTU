#include "gluit.h"
#include <stdio.h>
//#include "stlload.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

static STLmodel facet;
GLuint dl;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	glCallList (dl);

	glutSwapBuffers();
}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	// facet = stlRead ("../STLmodels/original.stl");
	facet = stlRead ("../STLmodels/james.stl");

	stlUnitize (facet);

	dl = stlRender2DL (facet);

	printf ("... done reading %d faces\n", facet.size());
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("STL read");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();

	glutMainLoop();
}
