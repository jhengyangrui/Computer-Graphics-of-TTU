#include "gluit.h"
#include "md2m.h"

// selected poses
#define STAND 0
#define RUN 1
#define JUMP 6
#define DEAD 17

int mainwin, viewer;
t3DModel ogro, weapon;
float center[3], size[3];  // of MD2 model


void display(void){}
void reshape(int w, int h) 
{
	glViewport (0,0,w,h);
	ViewerReshape (viewer);
}

void renderGrid (void)
{
#define MIN -10
#define MAX 10
#define STEP 1

	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_TEXTURE_2D);
	glColor3ub (255,0,0);
	glBegin (GL_LINES);
    for (int i = MIN; i <= MAX; i+= STEP) {
		glVertex3i (i, 0, MIN); glVertex3i (i, 0, MAX);
		glVertex3i (MIN, 0, i); glVertex3i (MAX, 0, i);
	}
	glEnd();
	glPopAttrib();
}


void content(void) 
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#define max(x,y) ((x) > (y) ? (x) : (y))

	float scale = max (max (size[0], size[1]), size[2]);

	glPushMatrix ();
    glTranslatef (0,1,0);
	glScalef (2/scale, 2/scale, 2/scale);
	glTranslatef (-center[0], -center[1], -center[2]);

    MD2MInterpolate (&ogro);
	MD2MInterpolate (&weapon);

    glPopMatrix();
	
	renderGrid();

 	glutSwapBuffers();
}



void init (void)
{
	MD2MLoad (&ogro, "../q2mdl/ogro/ogros.md2", "../q2mdl/ogro/igdosh.png");
	MD2MLoad (&weapon, "../q2mdl/ogro/weapon.md2", "../q2mdl/ogro/weapon.png");
//	MD2MLoad (&ogro, "../q2mdl/astro/astro.md2", "../q2mdl/astro/astro.bmp");
//	MD2MLoad (&weapon, "../q2mdl/astro/weapon.md2", "../q2mdl/astro/weapon.bmp");

	// find size based on ogro (weapon size with it)
	MD2MFindDimension (&ogro, center, size);

	MD2MSetAnimation (&ogro, STAND);
    MD2MSetLoop (&ogro, GL_TRUE);
	MD2MSetAnimationSpeed (&ogro,5.0);

	MD2MSetAnimation (&weapon, STAND);
    MD2MSetLoop (&weapon, GL_TRUE);
	MD2MSetAnimationSpeed (&weapon,5.0);

    glEnable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glClearColor (.5,.5,.5,0);
}

void idle(void)
{
	ViewerRedisplay (viewer);
}
void keyboard (unsigned char key, int x, int y)
{
    char strWindowTitle[50];
	static int modes = 0;
	
	switch (key) {	
	case 'a': case 'A':   // run all modes
 
		MD2MSetAnimation (&ogro, modes%ogro.numOfAnimations);
		MD2MSetAnimation (&weapon, modes%weapon.numOfAnimations);
		
		// Display the current animation in our window
		sprintf(strWindowTitle, "Md2 Animation: %s [%d]", 
				ogro.pAnimations[ogro.currentAnim].strName, ogro.currentAnim);
        glutSetWindow (mainwin);
		glutSetWindowTitle (strWindowTitle);
		glutSetWindow (viewer);
		modes++;
		break;
	}
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("ogro and weapon [A]");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init ();
	glutIdleFunc (idle);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}

