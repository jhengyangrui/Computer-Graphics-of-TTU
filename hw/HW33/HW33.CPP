#include <stdlib.h>
#include <gl/glut.h>
#include "svl/svl.h"
#pragma comment (lib, "svl-vc9.lib")

extern double l1, l2;

extern void screen2object (int,int,float*);
float target[3];

static int reachable = 1;
////////////////////////////////////////mouse position
Vec3 t;
Vec3 ballst = Vec3(0,0,-5);
///////////////////////////////////////
extern Vec3 takepoint;
//////////////////////////////////////
Vec3 speed1 = Vec3(0,0,0);
Vec3 speedball = Vec3(0,0,0);
////////////////////////////////////
Vec3 oldtakepoint;

/*void drawcircle (double r)
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
		glVertex3d (r*cos(theta*i),0, r*sin(theta*i));
	glEnd();

	glPopAttrib();
}*/
void drawball (float x, float z, float r)
{	
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	/*if (reachable) 
		glColor3ub (255,255,255);
	else
		glColor3ub (255,0,0);*/

	int lines = 360;
	int i;

	for(i = 0; i < lines; i++)
	{
		glBegin (GL_TRIANGLES);
			glVertex3f (x,0,z);
			glVertex3f (x+r*cos(i),0,z+r*sin(i));
			glVertex3f (x+r*cos(i+1),0,z+r*sin(i+1));
		glEnd();
	}
	glPopAttrib();
}
void coll()
{
	if(len(ballst - takepoint) < 1){
		speedball = speedball - dot(speedball - speed1,ballst - takepoint)/(len(ballst - takepoint)*len(ballst - takepoint))*(ballst - takepoint);
	}
	if(ballst[0] >= 9.5 && dot(speedball,Vec3(1,0,0)) > 0)
		speedball = speedball*Vec3(-1,1,1);
	if(ballst[0] <= -9.5 && dot(speedball,Vec3(-1,0,0)) > 0)
		speedball = speedball*Vec3(-1,1,1);
	if(ballst[2] <= -19.5 && dot(speedball,Vec3(0,0,-1)) > 0)
		speedball = speedball*Vec3(1,1,-1);
	if(ballst[2] >= -0.5 && dot(speedball,Vec3(0,0,1)) > 0)
		speedball = speedball*Vec3(1,1,-1);

}
void ballstate(int dummy)
{
	
	speed1 = (takepoint - oldtakepoint);
	oldtakepoint = takepoint;
	coll();
	ballst = ballst + speedball;

	if(len(speedball) > 0)
		speedball *= 0.995;
	if(len(speedball) < 0.1)
		speedball *= 0;

	if(len(ballst) > 10 && len(speedball) == 0){
		ballst = Vec3(0,0,-5);
		speedball *= 0;
	}

	glutTimerFunc (10, ballstate, 0);
	glutPostRedisplay();
}
void grid()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	glColor3f (1,1,1);
	glLineWidth (2.0);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i=i+1) {
		glVertex3i (-10,0,i); glVertex3i (10,0,i);
		glVertex3i (i,0,-10); glVertex3i (i,0,10);
	}
	glEnd();
	glPopAttrib ();
	
}
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	//drawcircle(l1+l2);
	glPushMatrix();	
	glTranslatef (0,0,-10);
	grid();
	glPopMatrix();

	extern void drawarm();
	drawarm();

	glPushMatrix();	

	glTranslatef (ballst[0],ballst[1],ballst[2]);
	drawball (0,0,.5);
	glPopMatrix();
	glutSwapBuffers();
}

void passive (int x, int y)
{
	/*Vec3 target;
	target[0] = (x-200)/200.*20;
	target[1] = (200-y)/200.*20; 
	target[2] = 0.0;*/
	
	screen2object (x, y, target);
    //printf ("%f %f %f\n", target[0],target[1],target[2]);
	t[0] = target[0];
	t[1] = 0; 
	t[2] = target[2];

	extern int myik (const Vec3&);
	reachable = myik (t);

	glutPostRedisplay();
}

void reshape (int w, int h)
{
	/*glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); gluOrtho2D (-20,20,-20,20);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();*/

	glViewport (0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (90, 1, .5, 100);
	glMatrixMode (GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt (0, 10, 0 ,  0,0, -5,  0,1,0);
}

void main (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize (400,400);
	glutCreateWindow ("IK (CCD)");

	glutPassiveMotionFunc (passive);
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
    //glEnable (GL_DEPTH_TEST);

	extern void setarm();
	setarm();
	glutTimerFunc (0, ballstate, 0);
	glutMainLoop ();
}
