#include "gluit.h"
#include "md2m.h"

// selected poses
#define STAND 0
#define RUN 1
#define JUMP 6
#define DEAD 17

int gpose = STAND;
int loopOver;
int help_on;

int mainwin, viewer, slider;
t3DModel g_3DModel;
float center[3], size[3];  // of MD2 model

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
#define max(a,b) ((a) > (b) ? (a) : (b))

	float scale = max (max (size[0], size[1]), size[2]);

	glPushMatrix ();
    glTranslatef (0,1,0);
	glScalef (2/scale, 2/scale, 2/scale);   // not 100% correct
	                                        // works for height is the largest dimension
	glTranslatef (-center[0], -center[1], -center[2]);

    loopOver = MD2MInterpolate (&g_3DModel);
    glPopMatrix();
	
	renderGrid();

    if (help_on) {
		help_screen ("[J]ump", "[R]un", "[D]ie", "[S]tand","[W]ire", "play [A]ll","fff");
	}

	glutSwapBuffers();
}


void display (void) {}
void reshape (int w, int h)
{
    glViewport (0,0,w,h);
    ViewerReshape (viewer);
	SliderReshape (slider);
}

void keyboard (unsigned char key, int x, int y)
{
    char strWindowTitle[50];
    static int wire = 0;
	static int modes = 0;

	switch (key) {
#if 1
	case 'w': case 'W':   // wire display
		wire ^= 1;
		if (wire) {
			MD2MSetViewMode (&g_3DModel,GL_LINE);
		} else {
			MD2MSetViewMode (&g_3DModel,GL_FILL);
		}
		break;
#endif
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

	case 'a': case 'A':   // run all modes

		MD2MSetAnimation (&g_3DModel, modes%g_3DModel.numOfAnimations);
	
		// Display the current animation in our window
		sprintf(strWindowTitle, "Md2 Animation: %s [%d]", 
				g_3DModel.pAnimations[g_3DModel.currentAnim].strName, g_3DModel.currentAnim);
        glutSetWindow (mainwin);
		glutSetWindowTitle (strWindowTitle);
		glutSetWindow (viewer);
		modes++;
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
    if (loopOver && gpose == JUMP) {
		// always resume stand (or previous action) after jump ...
		gpose = STAND;
		MD2MSetAnimation (&g_3DModel, gpose);
		MD2MSetLoop (&g_3DModel, GL_TRUE);
	}		

	//glutPostRedisplay();
	ViewerRedisplay (viewer);
}


static float animationspeed = 5.0;
void update (float value)
{
    //[0,1] -> [2, 42]
	animationspeed = value*40+2;
	MD2MSetAnimationSpeed (&g_3DModel,animationspeed);
	printf ("%f\n", animationspeed);
    ViewerRedisplay (viewer);
}

int main (int argc, char **argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize (400,500);

	mainwin = glutCreateWindow ("md2 animation");
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    
	viewer = ViewerNew (mainwin, 0,0,400,400, RenderScene);
    ViewerExtent (viewer, 5);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc (keyboard_s);

    glutIdleFunc (idle);

    //MD2MLoad(&g_3DModel, "tris.md2", "hobgoblin.bmp");
	//MD2MLoad(&g_3DModel, "q2mdl/astro/astro.md2", "q2mdl/astro/astro.bmp");
	//MD2MLoad(&g_3DModel, "../q2mdl/raptor/tris.md2", "../q2mdl/raptor/brown.bmp");
	//MD2MLoad(&g_3DModel, "../q2mdl/hand/tris.md2", "../q2mdl/hand/base.bmp");
	//MD2MLoad (&g_3DModel, "../q2mdl/abarlith/sfod8.md2", "../q2mdl/abarlith/abarlith.bmp");
	
	if (argc == 3) {
		MD2MLoad (&g_3DModel, argv[1], argv[2]);
	} else {
		MD2MLoad(&g_3DModel, "../q2mdl/laalaa/laalaa.md2", "../q2mdl/laalaa/laalaa.png");
		//printf ("exe md2 skin\n");
		//exit (1);
	}

	MD2MFindDimension (&g_3DModel, center, size);
	MD2MSetAnimation (&g_3DModel, STAND);
    MD2MSetLoop (&g_3DModel, GL_TRUE);
	
	MD2MSetAnimationSpeed (&g_3DModel,animationspeed);

	//glEnable (GL_LIGHTING); (lighting should be off)
	//glEnable (GL_LIGHT0);
    glEnable (GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glClearColor (.5,.5,.5,0);

	GLubyte rgb[3];
	rgb[0] = 255; rgb[1] = rgb[2] = 0;

	slider = SliderNew (mainwin, 0,400,400,100, rgb, 0.15, update);
    glutMainLoop();
}


	