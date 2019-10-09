//
// example of multiple md2 character (and other textures)
//

#include "gluit.h"
#include "md2m.h"
#include "vect.h"

// selected poses of raptor
#define STAND 0
#define RUN 1
#define JUMP 6
#define DEAD 20

// selected pose of laalaa
#define WAVE 8 

int gpose, gpose2;
int loopOver, loopOver2;
t3DModel g_3DModel, g_3DModel2;
float center[3], size[3];  // of MD2 model
float center2[3], size2[3]; 

int help_on;
int mainwin, viewer;

double theta;

extern void drawXtree (Vector);
extern void inittreetexture ( void);

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


void RenderScene(void) 
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This is where we call our animation function to draw and animate our character.
	// You can pass in any model into here and it will draw and animate it.  Of course,
	// it would be a good idea to stick this function in your model class.

	renderGrid();
	// xtree	
	VECTOR tcenter;
	SetVec (2., 0., 2., tcenter);
	drawXtree (tcenter);

	// raptor
#define max(x,y) ((x) > (y) ? (x) : (y))

	float scale = max (max (size[0], size[1]), size[2]);
    glPushMatrix ();
    glTranslatef (4*cos(theta), 0, -4*sin(theta));
    glRotatef (90+theta*180/3.1416, 0,1,0);

	glPushMatrix ();
    glTranslatef (0,2,0);
	glScalef (4/scale, 4/scale, 4/scale);
	glTranslatef (-center[0], -center[1], -center[2]);

    loopOver = MD2MInterpolate (&g_3DModel);
    glPopMatrix();
	glPopMatrix();
	

	// laalaa
	float scale2 = max (max (size2[0], size2[1]), size2[2]);
    glPushMatrix ();
	glTranslatef (4, 0, -4);
    glTranslatef (0,1,0);
	glScalef (2/scale2, 2/scale2, 2/scale2);
	glTranslatef (-center2[0], -center2[1], -center2[2]);

    loopOver2 = MD2MInterpolate (&g_3DModel2);
    glPopMatrix();

    if (help_on) {
		help_screen ("Raptor","[J]ump, [R]un","[D]ie, [S]tand","","Laalaa","[W]ave", "fff");
	}

	glutSwapBuffers();
}


//
// keyboard for:
// raptor run, jump, stand
// laalaa wave, stand
// 
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': case 'R': // run
		gpose = RUN;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_TRUE);
		break;
	case 's': case 'S': // stand
		gpose = STAND;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_TRUE);
		break;
	case 'j': case 'J': // jump   
		gpose = JUMP;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_FALSE);
		break;
	case 'd': case 'D': // dead
		gpose = DEAD;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_FALSE);
		break;
	case 'w': case 'W':   // laalaa wave
		gpose2 = WAVE;
		MD2MSetAnimation (&g_3DModel2, gpose2);
		MD2MSetLoop (&g_3DModel2, GL_FALSE);
		break;
	default:  // any key to remove help  
		if (help_on == 1) {
			glutPostRedisplay();
			help_on = 0;
		}
		break;
	}
}


void keyboard_s (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_F1: // F1 for help screen
			help_on = 1;
			glutPostRedisplay();
			break;
		default:
			if (help_on == 1) {
				glutPostRedisplay();
				help_on = 0;
			}
			break;
	}
}

void idle (void)
{ 
    static int last;
	int now;
    float dt;

	now = glutGet (GLUT_ELAPSED_TIME);
	dt = (now-last)/1000.;

	if (gpose == RUN)
		theta += 1*dt;

    if (loopOver && gpose == JUMP) {
		// always resume stand (or previous action) after jump ...
		gpose = STAND;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_TRUE);
	}		

    if (loopOver2 && gpose2 == WAVE) {
		// always resume stand (or previous action) after laalaa wave ...
		gpose2 = STAND;
		MD2MSetAnimation (&g_3DModel2, gpose2);
		MD2MSetLoop (&g_3DModel2, GL_TRUE);
	}		

//	glutPostRedisplay();
	ViewerRedisplay (viewer);
	last = now;
}

void display (void) {}
void reshape (int w, int h)
{
    glViewport (0,0,w,h);
    ViewerReshape (viewer);
}

int main (int argc, char **argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize (400,400);

	mainwin = glutCreateWindow ("md2 animation");
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    
	viewer = ViewerNew (mainwin, 0,0,400,400, RenderScene);
    ViewerExtent (viewer, 12);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc (keyboard_s);

    glutIdleFunc (idle);

	inittreetexture ();

    //MD2MLoad(&g_3DModel, "tris.md2", "hobgoblin.bmp");
	//MD2MLoad(&g_3DModel, "q2mdl/astro/astro.md2", "q2mdl/astro/astro.bmp");
	MD2MLoad(&g_3DModel, "../q2mdl/raptor/tris.md2", "../q2mdl/raptor/brown.png");
	//MD2MLoad(&g_3DModel, "q2mdl/jeep/tris.md2", "q2mdl/jeep/jeep1.bmp"); does not work
	
	MD2MFindDimension (&g_3DModel, center, size);
	MD2MSetAnimation (&g_3DModel, STAND);
    MD2MSetLoop (&g_3DModel, GL_TRUE);
	
	MD2MLoad(&g_3DModel2, "../q2mdl/laalaa/laalaa.md2", "../q2mdl/laalaa/tinkywinky.png");
	MD2MFindDimension (&g_3DModel2, center2, size2);
	MD2MSetAnimation (&g_3DModel2, STAND);
    MD2MSetLoop (&g_3DModel2, GL_TRUE);

	MD2MSetAnimationSpeed (&g_3DModel,15.0);
	MD2MSetAnimationSpeed (&g_3DModel2,5.0);

	//glEnable (GL_LIGHTING); (lighting should be off)
	//glEnable (GL_LIGHT0);
    
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glClearColor (.5,.5,.5,0);

    glutMainLoop();
}


	