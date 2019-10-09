#include <gl/glut.h>

extern void ProcessCapture(int,int=10);

//#pragma comment (lib, "gifcapture.lib")
#pragma comment (lib, "gifcapture_MD.lib")

static int ww=512,wh=512;
static float spin;

void drawteaport()
{
	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(0,2.25,0);
	glRotatef(spin,0,1,0);
	glutSolidTeapot(3.0);
	glPopMatrix();
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10,10,0,0,0,0,0,1,0);

	drawteaport();
	
	glutSwapBuffers();
}

void spinDisplay (int dummy)
{	
	spin = spin + 6;
	if (spin > 360.0)
      spin = spin - 360.0;
	glutTimerFunc (10, spinDisplay, 0);
    glutPostRedisplay();
}

void reshape(int w,int h) 
{
	ww=w; wh=h;
	glViewport(0,0,ww,wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float fovy = 70;
	gluPerspective(fovy,1,1,50);
}


void special (int key, int x, int y)
{
	if (key == GLUT_KEY_HOME) {
		ProcessCapture(glutGetWindow ());
	}
}

void init(void) 
{	
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

}


void main(int argc, char** argv) 
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(ww,wh); 
	glutInitWindowPosition(100,100);
	glutCreateWindow("[Home] to start/stop capture");
	
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutTimerFunc (10, spinDisplay, 0);	
	glutMainLoop();
}