#include "gluit.h"

#include <stdio.h>
#pragma comment (lib, "gluit-vc9.lib")

extern void playex (int);

extern void init_fmod();
extern void shutdown_fmod();

static int choice = 0;
void display()
{
	glClear (GL_COLOR_BUFFER_BIT); 

	setfont ("9x15",0);
	glColor3f (1,1,0);
	BEGIN_2D_OVERLAY(20,20);
	drawstr (2,3, "1: Machine gun");
	drawstr (2,2, "2: Cannon");
	drawstr (2,1, "3: Laser");
	if (choice) drawstr (1, 4-choice, "v");
	END_2D_OVERLAY();

	glFlush();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		choice = 1;
		break;
	case '2':
		choice = 2;
		break;
	case '3':
		choice = 3;
		break;
	default:
		return;
	}
	playex(choice);
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitWindowSize (300,300);
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow ("Shoot");

	glutDisplayFunc (display);
	glutKeyboardFunc (keyboard);

	init_fmod();

	glutMainLoop();
	shutdown_fmod();
}

