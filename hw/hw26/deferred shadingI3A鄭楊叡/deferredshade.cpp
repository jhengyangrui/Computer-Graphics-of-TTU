#include <gl/glew.h>
#include "gluit.h"
#include "stdio.h"
#include "windows.h"
#pragma comment (lib, "glew32.lib")

// first pass render diffuse, normal, depth
//   no specular yet, (can be added)
//   (restricted to directional light?  unless we add a position map? )
//   can add SSAO effects...
 
// second pass
// render a screen space quad
// each fragment generates its due shades

// first, see if we can write diffuse to a texture ...

LARGE_INTEGER freq, beforeA, afterA, beforeB, afterB;

int mainwin, viewer;
int width, height;
int toggle = 1;

void display(){}
void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

GLuint mrtshader, defershader;

void scene()
{
	float red[] = {1., 0., 0., 1.};
	float yellow[] = {1., 1., 0., 1.};
	float green[] = {0., 1., 0., 1.};
	float blue[] = {0., 0., 1., 1.};
	float cyan[] = {0., 1., 1., 1.};
	float black[] = {0., 0., 0., 1.};
	float white[] = {0.5, 0.5, 0.5, 1.};

	
	glMaterialfv (GL_FRONT, GL_SPECULAR, white);
	glMaterialf (GL_FRONT, GL_SHININESS, 100);

		glPushMatrix();

	for(int i = 0; i < 5; i++)
	{
		

		glPushMatrix();
		glTranslatef (-6,0,0);
		glRotatef (30,0,1,0);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, red);
		glMaterialfv (GL_FRONT, GL_AMBIENT, black);
		glutSolidTeapot(1.0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef (-3,0,0);
		glRotatef (30,0,1,0);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, yellow);
		glutSolidTeapot(1.0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef (0,0,0);
		glRotatef (30,0,1,0);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, green);
		glutSolidTeapot(1.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef (3,0,0);
		glRotatef (30,0,1,0);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, cyan);
		glutSolidTeapot(1.0);
		glPopMatrix();

		//glPushAttrib();
		glPushMatrix();
		glTranslatef (6,0,0);
		glRotatef (30,0,1,0);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, blue);
		glutSolidTeapot(1.0);
		glPopMatrix();

		glTranslatef (0,0,-2);
	}

		glPopMatrix();
	//glPopAttrib();

}

float theta;

void content()
{
	char s[20];

	glViewport (0,0,400,400);
	// Render-to-texture pass
	extern GLuint fbo;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);	

	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	gluPerspective (65, 1, 1,40);  // a narrower depth range
	glMatrixMode (GL_MODELVIEW);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	if(toggle == 1)
		QueryPerformanceCounter(&beforeA);
	glUseProgram (mrtshader);
	scene();

	glUseProgram(0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// deferred shade pass
	glViewport (0,0,400,400);  // clear full screen
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef (theta, 0,1,0);
	float pos[] = {.1,.1,.1,0};
	glLightfv (GL_LIGHT0, GL_POSITION, pos);
	glPopMatrix();

	
	glViewport (0,0,400,400);

	glUseProgram (defershader);
	glUniform1i (glGetUniformLocation (defershader, "img_diffuse"), 0);
	glUniform1i (glGetUniformLocation (defershader, "img_normal"), 1);
	glUniform1i (glGetUniformLocation (defershader, "img_depth"), 2);
	
	extern GLuint img_diffuse, img_depth, img_normal;

	glActiveTexture (GL_TEXTURE0); glBindTexture (GL_TEXTURE_2D, img_diffuse);
	glActiveTexture (GL_TEXTURE1); glBindTexture (GL_TEXTURE_2D, img_normal);
	glActiveTexture (GL_TEXTURE2); glBindTexture (GL_TEXTURE_2D, img_depth);

	BEGIN_2D_OVERLAY (10,10);
	glBegin (GL_QUADS);  // full-screen rendering
	glTexCoord2f (0,0); glVertex2i (0,0);
	glTexCoord2f (1,0); glVertex2i (10,0);
	glTexCoord2f (1,1); glVertex2i (10,10);
	glTexCoord2f (0,1); glVertex2i (0,10);
	glEnd();
	END_2D_OVERLAY();

	glUseProgram(0);
	if(toggle == 1)
		QueryPerformanceCounter(&afterA);

	// regular rendering
	// for comparison
	glViewport (0,0,400,400);
	if(toggle == -1) {
		QueryPerformanceCounter(&beforeB);
		scene();
		QueryPerformanceCounter(&afterB);		
		BEGIN_2D_OVERLAY(10,10);
			drawstr (1,1, "no deferred");
		END_2D_OVERLAY();
	}

	if(toggle == 1)
		sprintf(s, "%lf\n", (afterA.QuadPart-beforeA.QuadPart)/(double)freq.QuadPart);
	else
		sprintf(s, "%lf\n", (afterB.QuadPart-beforeB.QuadPart)/(double)freq.QuadPart);
	
	BEGIN_2D_OVERLAY(10,10);
		drawstr (1,9, s);
	END_2D_OVERLAY();

	glutSwapBuffers();
}


void init()
{
	glewInit(); 

	glClearColor (1,1,1,1);

	extern void initFBO();
	initFBO();
	
	glEnable (GL_DEPTH_TEST);	
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	float pos[] = {1,1,1,0};
	glLightfv (GL_LIGHT0, GL_POSITION, pos);

	extern GLuint setShaders (char*, char*);
	mrtshader = setShaders ("mrt.vert", "mrt.frag");
	defershader = setShaders ("defershade.vert", "defershade.frag");

	QueryPerformanceFrequency(&freq);
}

void timer (int dummy)
{
	glutTimerFunc (100, timer, 0);
//	theta += 2;
	glutPostRedisplay();
}


void keyboard (unsigned char key, int x, int y)
{

	if (key == ' ')
	{
		toggle  *= -1;

	}

	glutPostRedisplay();
}

void main (int argc, char** argv)
{ 
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("deferred shading [t]");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,800,400, content);
	init();
	ViewerExtent (viewer, 10.0);
	glutTimerFunc (10, timer, 0);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}
