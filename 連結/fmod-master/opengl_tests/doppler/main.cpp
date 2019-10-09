#include "gluit.h"
#include "svl/svl.h"

extern void Update (const Vec3&, const Vec3&);
extern void init_fmod ();

int mainwin, viewer;
void display(void){}
void reshape (int w, int h) 
{
	glViewport (0,0,w,h);
	ViewerReshape (viewer);
}

void grid (void)
{
	int i;
#define MIN -10
#define MAX 10
	glColor3ub (255,255,255);
	glBegin (GL_LINES);
	for(i = MIN; i <= MAX; i++) {
		glVertex3i (i,0,MAX); glVertex3i (i,0,MIN);
		glVertex3i (MIN,0,i); glVertex3i (MAX,0,i);
	}
	glEnd();
}

Vec3 siren, vel;

void init (void)
{
	siren = Vec3 (-18., 0., 15.);    // the line of z+x=-3
	vel = Vec3 (3., 0., -3.);
}

void content (void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	grid ();
	
	glBegin (GL_TRIANGLES);
	glVertex3f (0.5, 0,0);
	glVertex3f (0,0,-2);
	glVertex3f (-0.5,0,0);
	glEnd();

	glPointSize (12.0);
	glColor3ub (255,0,0);
	glBegin (GL_POINTS);
	glVertex3dv (siren.Ref());
	glEnd();

	glutSwapBuffers();
}

void idle (void)
{
	static int last;
	int now;
	double dt;

	if (last == 0) {
		last = glutGet (GLUT_ELAPSED_TIME);
		return;
	}
	now = glutGet (GLUT_ELAPSED_TIME);
	dt = (now-last)/1000.;
	last = now;

	siren += dt*vel;

	if (fabs(siren[0]) > 20)
		vel *= -1;

	Update (siren, vel);
	ViewerRedisplay (viewer);
}

void main (int argc, char** argv) 
{
	glutInit (&argc, argv);
	glutInitWindowSize (400,400);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	mainwin = glutCreateWindow ("doppler");

	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	glutIdleFunc (idle);
	init();
	init_fmod();
	ViewerExtent (viewer, 10.0);

	glutMainLoop();
}