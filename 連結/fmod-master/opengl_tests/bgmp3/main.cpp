#include "gluit.h"

#pragma comment (lib, "gluit-vc9.lib")

extern void play();
extern void playstream();
extern void togglesong();

extern void init_fmod();
extern void shutdown_fmod();

static int choice = 0;
void display()
{
	glClear (GL_COLOR_BUFFER_BIT);

	setfont ("9x15",0);
	glColor3f (1,1,0);
	BEGIN_2D_OVERLAY(20,20);
	drawstr (2,5, "1: play SOUND");
	drawstr (2,4,"2: play STREAM");
	drawstr (2,3,"P: toggle song");
	
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
		playstream();
		choice = 2;
		break;
	case 'P': case 'p':
		togglesong ();
		choice = 3;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitWindowSize (300,300);
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow ("BG MP3");

	glutDisplayFunc (display);

	glutKeyboardFunc (keyboard);

	init_fmod();

	glutMainLoop();
	shutdown_fmod();
}

