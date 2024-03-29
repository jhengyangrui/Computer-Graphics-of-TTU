#include "gluit.h"

#pragma comment (lib, "gluit-vc9.lib")

extern void play();
extern void playex();
extern void freq_up();
extern void freq_down();
extern void toggle_pause();
extern void stopsound();

extern void init_fmod();
extern void shutdown_fmod();

static int choice = 0;

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);

	setfont ("9x15",0);
	glColor3f (1,1,0);
	BEGIN_2D_OVERLAY(20,20);
	drawstr (2,5, "1: single play");
	drawstr (2,4,"2: loop play");
	drawstr (2,3,"P: toggle pause");
	drawstr (2,2,"S: stop");
	drawstr (2,1,"Up/Down: +/- semitone");

	if (choice) drawstr (1, 6-choice, "v");
	END_2D_OVERLAY();
	glFlush();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		play();
		choice = 1;
		break;
	case '2':
		playex();
		choice = 2;
		break;
	case 'p': case 'P':
		toggle_pause();
		choice = 3;
		break;
	case 's': case 'S':
		stopsound();
		choice = 4;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void special (int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		freq_up();
		choice = 5;
		break;
	case GLUT_KEY_DOWN:
		freq_down();
		choice = 5;
		break;
	}
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitWindowSize (300,300);
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow ("Basic FMOD");

	glutDisplayFunc (display);
	glutSpecialFunc(special);
	glutKeyboardFunc (keyboard);

	init_fmod();

	glutMainLoop();
	shutdown_fmod();
}

