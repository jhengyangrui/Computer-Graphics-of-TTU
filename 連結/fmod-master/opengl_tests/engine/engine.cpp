#include "gluit.h"
#include <stdio.h>
#pragma comment (lib, "gluit-vc9.lib")

extern void playex();
extern void play_freq (double);

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
	drawstr (2,5, "1: Start engine");
	drawstr (2,4,"UP: Step on it!");
	drawstr (2,3,"S: stop");

	if (choice) drawstr (1, 6-choice, "v");
	END_2D_OVERLAY();

	printf ("%d\n", choice);
	glFlush();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		playex();
		choice = 1;
		break;
	case 's':
		stopsound();
		choice = 3;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

double clamp (double x, double lo, double hi)
{
	return (x > hi) ? hi : ((x < lo) ? lo : x);
}

static double freq = 1.0;
void special (int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		freq *= 1.1;
		freq = clamp (freq, 1.0, 5.0);
		play_freq(freq);
		choice = 2;
		break;
	}
	glutPostRedisplay();
}

void timer (int dummy)
{
	freq /= 1.1;
	freq = clamp (freq, 1.0, 5.0);

	glutTimerFunc (50, timer, 0);
	play_freq (freq);
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitWindowSize (300,300);
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow ("fmod Engine");

	glutDisplayFunc (display);
	glutSpecialFunc(special);
	glutKeyboardFunc (keyboard);
	glutTimerFunc (0, timer, 0);

	init_fmod();

	glutMainLoop();
	shutdown_fmod();
}

