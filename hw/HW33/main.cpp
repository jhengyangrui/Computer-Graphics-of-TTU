#include <stdlib.h>
#include <gl/glut.h>
#include "svl/svl.h"
#pragma comment (lib, "svl-vc9.lib")

extern double l1, l2;

static int reachable = 1;

void drawcircle (double r)
{
#define SEGS 20

	glPushAttrib (GL_ALL_ATTRIB_BITS);

	if (reachable) 
		glColor3ub (255,255,255);
	else
		glColor3ub (255,0,0);

	glLineWidth (1.0);

	glBegin (GL_LINE_LOOP);
	double theta = 2*3.1416/SEGS;
	for (int i = 0; i < SEGS; i++) 
		glVertex2d (r*cos(theta*i), r*sin(theta*i));
	glEnd();

	glPopAttrib();
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	drawcircle(l1+l2);

	extern void drawarm();
	drawarm();

	glutSwapBuffers();
}

void passive (int x, int y)
{
	Vec3 target;
	target[0] = (x-200)/200.*20;
	target[1] = (200-y)/200.*20; 
	target[2] = 0.0;

	extern int myik (const Vec3&);
	reachable = myik (target);

	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); gluOrtho2D (-20,20,-20,20);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
}

void main (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize (400,400);
	glutCreateWindow ("IK (CCD)");

	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutPassiveMotionFunc (passive);

	extern void setarm();
	setarm();
	
	glutMainLoop ();
}
