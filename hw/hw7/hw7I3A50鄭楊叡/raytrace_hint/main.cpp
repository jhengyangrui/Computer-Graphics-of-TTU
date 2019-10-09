#include "gluit.h"
#include <svl/svl.h>


extern void DoRayTrace(void);  //ray.cpp

static int mainwin, viewer;
static int raycast;

//
// global data
//

int maxlevel = 3;   // max level of reflection
int numobjects = 3;  // id: 1,2,3
int numlights = 1;
Vec3 lightpos[1]; 
Vec3 camera;

int gw =256;
int gh =256;
void display(void){}

#define NSPHERES 120
float pos[NSPHERES][3];
float radius[NSPHERES];
float theta = 0;
int X , Y ;
static int content_count;  // for FPS computation
static int cull;

void reshape (int w, int h)
{
	gw = w, gh = h;
	glViewport (0,0,w,h);
	ViewerReshape(viewer);
}

void DisplayImage(void)
{
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity();
	gluOrtho2D (0,20,0,20);  // this doesnt really matter: we draw an image
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	glRasterPos2i (0,0);

	extern GLubyte image[256][256][3];
	//
	// use pixel zoom to account for reshape 	
	//
	glPixelZoom(X, Y);

	glDrawPixels (256,256, GL_RGB, GL_UNSIGNED_BYTE, image);
		
	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix(); 
}


static void hint(void)
{
	BEGIN_2D_OVERLAY (20,20);
	
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glColor3f (0.88, 0.02, 0.89);
	setfont ("8x13",0);
	drawstr (1,19, "r to switch  a to rotate");
	glPopAttrib();

	END_2D_OVERLAY();
}

void draw3Dscene()
{
	float white[4] = {1,1,1,1};
	float yellow[4] = {1,1,0,1};
	glMaterialfv (GL_FRONT, GL_DIFFUSE, yellow);
	glMaterialfv (GL_FRONT, GL_SPECULAR, white);
	glMateriali (GL_FRONT, GL_SHININESS, 40);
	float lpos[4] = {lightpos[0][0], lightpos[0][1], lightpos[0][2], 1.0};
	glPushMatrix ();
	glRotatef (theta, 0,1,0); 
	glLightfv (GL_LIGHT0, GL_POSITION, lpos);    // point light
	glPopMatrix();
	glEnable (GL_NORMALIZE);
	glPushMatrix ();
		glScalef (2., 1., 1.);
		glutSolidSphere (1.0, 20,20);
	glPopMatrix();
	float grey[] = {.2,.2,.2};
	float purple[] = {.5,0,.5};
	float green[] = {0,.7,0};
	glMaterialfv (GL_FRONT, GL_AMBIENT, grey); 
	glMaterialfv (GL_FRONT, GL_DIFFUSE, purple);
	glMaterialfv (GL_FRONT, GL_SPECULAR, green);
	glMateriali (GL_FRONT, GL_SHININESS, 10);
	glBegin (GL_QUADS);
		glNormal3f(0,1,0);
		glVertex3i (-4,-2,4);
		glVertex3i (4,-2,4);
		glVertex3i (4,-2,-4);
		glVertex3i (-4,-2,-4);
	glEnd();
}

void content (void)
{
	hint();
	if (raycast) {
		DoRayTrace ();
		DisplayImage ();
	} else {
		draw3Dscene();
		
		// draw light
		glPushAttrib (GL_ENABLE_BIT);
		glDisable (GL_LIGHTING);
		glColor3f (1,1,0);
		glPushMatrix();
		glRotatef (theta, 0,1,0);            ////////************
		glTranslated (lightpos[0][0], lightpos[0][1], lightpos[0][2]);
		glutWireSphere (0.1,12,12);
		glPopMatrix();
		glPopAttrib();
	}

	//
	// FPS calculation and output
	///////////////////////////////////////////////////////////
	int nshow = 0;

	BEGIN_2D_OVERLAY (10,10);
	glDisable(GL_LIGHTING);
	glColor3f (1,1,0);
	if (cull) 	
		drawstr (1,1, "cull [%d|%d]", NSPHERES-nshow, NSPHERES);
	else
		drawstr (1,1, "no cull");
	END_2D_OVERLAY();

	content_count++;

	glutSwapBuffers();
}

void init (void)
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_NORMALIZE);

	lightpos[0] = Vec3 (0,5,5);
}
void timer(int dummy)
{
	char msg[50];
	sprintf (msg, "FPS: %d\n", content_count);

	glutSetWindow (mainwin);
	glutSetWindowTitle (msg);
	glutSetWindow (viewer);

	content_count = 0;
	glutTimerFunc (1000, timer, 0);
}
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': case 'R': 
		raycast ^= 1;
		ViewerRedisplay(viewer);
		break;	
	case 'a': case 'A': 
		theta += 1;
		ViewerRedisplay(viewer);
		break;
	}

}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (256,256);

	mainwin = glutCreateWindow ("raycast");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin,0,0,256,256, content);
	glutKeyboardFunc (keyboard);
	glutTimerFunc (1000, timer, 0);
	glutIdleFunc (content);

	init();
	glutMainLoop();
	return 0;
}