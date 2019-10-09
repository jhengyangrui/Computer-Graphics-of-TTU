#include <gl/glut.h>
#include <vector>
#include "svl/svl.h"

using namespace std;

vector<Vec2> points, chpts;

void PrintVector(const vector<Vec2>& tt)
{
	for (int i =0; i < tt.size(); i++)
		cout << tt[i] << endl;
}

int chdone = 0;
void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glBegin (GL_POINTS);
	for (int i = 0; i < points.size(); i++) {
		glVertex2dv (points[i].Ref());
	}
	glEnd();

	if (chdone) {
		glColor3f (1,1,0);
		glBegin (GL_LINE_LOOP);
		for (int i = 0; i < chpts.size(); i++) {
			glVertex2dv (chpts[i].Ref());
		}
		glEnd();
	}

	glutSwapBuffers();
}

void init()
{
	// pipeline
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-10,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	glViewport (0,0,400,400);
	
	glClearColor (.4,.4,.4,1);
	glColor3f (1,0,0);
	glPointSize (10);
}

void mouse (int button, int state, int mx, int my)
{
	if (state == GLUT_UP) return;
	if (button == GLUT_LEFT_BUTTON) {
		float x = mx/400.*20 - 10;
		float y = 10 - my/400.*20;

		points.push_back (Vec2 (x,y));
	} else if (button == GLUT_RIGHT_BUTTON) {
		extern vector<Vec2> CHull (const vector<Vec2>&);
		chpts = CHull (points);
		chdone = 1;
	}
	glutPostRedisplay();
}


void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("tst");
	
	glutDisplayFunc (display);
	glutMouseFunc (mouse);
	init();
	glutMainLoop();
}
