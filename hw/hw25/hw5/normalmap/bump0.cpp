#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"
#include "svl/svl.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "svl-dbg.lib")


int mainwin, viewer;
float theta = 0;

void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}
GLuint p, colortex, normaltex;
GLuint tan_loc, bitan_loc;

void grid()
{
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	glColor3f(1,1,1);
	glBegin (GL_LINES);
	for (int i =-5; i <= 5; i++) {
		glVertex3i (-5, 0, i); glVertex3i (5,0,i);
		glVertex3i (i,0,-5); glVertex3i(i,0,5);
	}
	glEnd();
	glPopAttrib();
}
GLuint nobumploc;
int bumptoggle;

void wall()
{
	glBegin (GL_QUADS);
		// same attributes for every vertex
		glVertexAttrib3f(tan_loc,1,0,0);
		glVertexAttrib3f(bitan_loc, 0,1,0);
		glNormal3f (0,0,1);
		
		glMultiTexCoord2f (GL_TEXTURE0, 0,0); glVertex3i (-2, 0, 0); 
		glMultiTexCoord2f (GL_TEXTURE0, 1,0); glVertex3i (2,0,0);
		glMultiTexCoord2f (GL_TEXTURE0, 1,1); glVertex3i (2,4,0);
		glMultiTexCoord2f (GL_TEXTURE0, 0,1); glVertex3i (-2,4,0);
	glEnd();
}

void content()
{
	Vec3 lightpos = Vec3 (-2, 2, 2);
	Vec3 lpos = Rot3 (Vec3 (0, 1, 0), theta) * lightpos;
	float pos[4];
	pos[0] = lpos[0];
	pos[1] = lpos[1];
	pos[2] = lpos[2];
	pos[3] = 1.0;

	/*float pos[4];
	pos[0] = 3;
	pos[1] = 2;
	pos[2] = 1;
	pos[3] = 1.0;*/

	glLightfv (GL_LIGHT0, GL_POSITION, pos);

	float white[] = {1,1,1,1};
	glLightfv (GL_LIGHT0, GL_AMBIENT, white);

	glUseProgram(p);
	wall();

	glUseProgram(0);
	grid();
	glDisable(GL_LIGHTING);
	glColor3ub (255,255,0); 
	glTranslatef (pos[0],pos[1],pos[2]);
	glutWireSphere(0.2, 20, 20);

	glutSwapBuffers();
}

int animate = 0;

void init()
{
	glEnable (GL_DEPTH_TEST);
	glClearColor (.4,.4,.4,1);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);


	glewInit();
	extern GLuint setShaders(char*,char*);
	p = setShaders ("bumpmap.vert", "bumpmap.frag");
	pngSetStandardOrientation(1);
	colortex = pngBind("rock.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	normaltex = pngBind("rockDOT3.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	glUseProgram (p);
	glUniform1i (glGetUniformLocation (p,"NormalMap"), 0);
	glUniform1i (glGetUniformLocation (p,"ColorMap"), 1);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, normaltex);
	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, colortex);
	
	tan_loc = glGetAttribLocation (p,"Tangent");
	bitan_loc = glGetAttribLocation (p,"Bitangent");	
}

float a = 0;
void timer (int dummy)
{
	theta = theta + (0.1 * a);
	if (theta <= 0.0){
		a = 1;printf("1");
	}
	if (theta >= 2){
		a = -1;printf("-1");
	}
	glutPostRedisplay();
	glutTimerFunc (100, timer, 0);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case ' ': // switch
		animate ^= 1;

		glUseProgram (p);
		glUniform1i (glGetUniformLocation (p,"nobump"), animate);
		break;
	}
}
void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("bump (b,SPC)");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();	
	glutKeyboardFunc(Keyboard);
	glutTimerFunc (100, timer, 0);

	glutMainLoop();
}
