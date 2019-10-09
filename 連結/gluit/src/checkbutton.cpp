#include <stdio.h>
#include "gluit.h"

typedef struct _checkbutton {
    int window;
    int parentwin;
    int x,y,w,h;
    float xP,yP,wP,hP;
    
    char text[11];   // annotation
    GLboolean *varptr;  // toggle a global boolean variable
	int viewer;   // the viewer got affected
    
    struct _checkbutton *next;
} CheckButton;

//#define NULL 0
static CheckButton* _cbuttons = NULL;
static CheckButton* _cbuttonHit = NULL;

#define CBUTTON_TRUE  1
#define CBUTTON_FALSE 2

static void 
_cbuttonInit()
{
    int p1[2], p2[2], p3[2], p4[2], p5[2], p6[2], p7[2], p8[2];
#define PAD 1

	p1[0] = 1, p1[1] = 1;
	p2[0] = 7, p2[1] = 1;
	p3[0] = 7, p3[1] = 7;
	p4[0] = 1, p4[1] = 7;

	p5[0] = p1[0]+PAD, p5[1] = p1[1]+PAD;
	p6[0] = p2[0]-PAD, p6[1] = p2[1]+PAD;
	p7[0] = p3[0]-PAD, p7[1] = p3[1]-PAD;
	p8[0] = p4[0]+PAD, p8[1] = p4[1]-PAD;

	glNewList (CBUTTON_FALSE, GL_COMPILE);   // unset 
	glBegin (GL_QUADS);
	    glColor3f (0.91,0.90,0.4);
	    glVertex2iv (p1); glVertex2iv (p5);glVertex2iv (p8);glVertex2iv (p4);
        glVertex2iv (p8); glVertex2iv (p7);glVertex2iv (p3);glVertex2iv (p4);
 
	    glColor3f (0.81,0.8,0.35);
		glVertex2iv (p5); glVertex2iv (p6);glVertex2iv (p7);glVertex2iv (p8);
	    
		glColor3f (0.67,0.67,0.29);
		glVertex2iv (p1); glVertex2iv (p2);glVertex2iv (p6);glVertex2iv (p5);
        glVertex2iv (p2); glVertex2iv (p3);glVertex2iv (p7);glVertex2iv (p6);

	glEnd();
	glEndList();

    glNewList (CBUTTON_TRUE, GL_COMPILE);
	glBegin (GL_QUADS);
		glColor3f (0.67,0.67,0.29);
	    glVertex2iv (p1); glVertex2iv (p5);glVertex2iv (p8);glVertex2iv (p4);
        glVertex2iv (p8); glVertex2iv (p7);glVertex2iv (p3);glVertex2iv (p4);
 
	    glColor3ub (120,0,0);
		glVertex2iv (p5); glVertex2iv (p6);glVertex2iv (p7);glVertex2iv (p8);
	    
	    glColor3f (0.91,0.90,0.4);
		glVertex2iv (p1); glVertex2iv (p2);glVertex2iv (p6);glVertex2iv (p5);
        glVertex2iv (p2); glVertex2iv (p3);glVertex2iv (p7);glVertex2iv (p6);

	glEnd();
	glEndList();
}

//
static CheckButton* 
_cbuttonCurrent (void)
{
    CheckButton *cbutton = _cbuttons;
    
    int window = glutGetWindow ();
    
    while (cbutton) {
    	if (cbutton->window == window)
    	    break;
    	cbutton = cbutton->next;
    }
    if (! cbutton)
        printf ("error ...\n");
    return cbutton;
}

//static int mode = CBUTTON_RELEASE;
static void
_cbuttonMouse (int cbutton, int state, int x, int y)
{
    CheckButton *thecbutton = _cbuttonCurrent();
    
    _cbuttonHit = thecbutton;
    switch (cbutton) {
	case GLUT_LEFT_BUTTON:
    	if (state == GLUT_DOWN) {
			*(thecbutton->varptr) ^= 1;
			ViewerRedisplay(thecbutton->viewer);
        } else if (state == GLUT_UP) {
		}
		glutSetWindow (thecbutton->window);
		glutPostRedisplay ();
	break;
    }
}

static void 
_cbuttonDisplay (void)
{
    CheckButton *cbutton = _cbuttonCurrent();

	glViewport (0,0,cbutton->w, cbutton->h);
    
	glClearColor (0.4,0.4,0.29,0);
	glClear (GL_COLOR_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (0,32,0,8);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();

	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	if (*(cbutton->varptr ) == GL_TRUE)
		glCallList (CBUTTON_TRUE);
	else
		glCallList (CBUTTON_FALSE);

	drawstr (8,3,cbutton->text);

	glPopAttrib();
	glutSwapBuffers();

	glutSetWindow (cbutton->parentwin);   // for idling to continue;
}

//
static CheckButton*
_cbuttonSelect (int win)
{
    CheckButton *cbutton = _cbuttons;
    
    while (cbutton) {
    	if (cbutton->window == win)
    	   break;
    	cbutton = cbutton->next;
    }

    if (! cbutton)
       printf ("error...\n");

    return cbutton;
}
//    
void CheckButtonReshape (int win)
{
    CheckButton *cbutton = _cbuttonSelect (win);  // get the viewer correspond to win (id)
    int view[4];  
    int parentwin = cbutton->parentwin;

	glutSetWindow (parentwin);
	glGetIntegerv (GL_VIEWPORT, view);

	//
	// reposition in the parent
	glutSetWindow (win);

	cbutton->x = view[2]*cbutton->xP;
	cbutton->y = view[3]*cbutton->yP;
	cbutton->w = view[2]*cbutton->wP;
	cbutton->h = view[3]*cbutton->hP;

	if (cbutton->w && cbutton->h) {// w & h become 0 when window is minimized
	glutPositionWindow (cbutton->x, cbutton->y);
	glutReshapeWindow (cbutton->w, cbutton->h);
	glutPostRedisplay ();
	}
}

//
int CheckButtonNew (
int parent, int x, int y, int w, int h, 
char *text, GLboolean *varptr, int viewer)
{
    int view[4];
    CheckButton *cbutton = (CheckButton*) malloc (sizeof (CheckButton));
    cbutton->next = _cbuttons;
    _cbuttons = cbutton;
    
    cbutton->window = glutCreateSubWindow (parent, x,y,w,h);
    cbutton->parentwin = parent;
    cbutton->x = x; cbutton->y = y;
    cbutton->w = w; cbutton->h = h;
	
	strcpy (cbutton->text, text);
    _cbuttonInit();
    
    cbutton->varptr = varptr;
	cbutton->viewer = viewer;
    glutDisplayFunc (_cbuttonDisplay);
    glutMouseFunc (_cbuttonMouse);
    glutSetCursor (GLUT_CURSOR_CROSSHAIR);
    
    glutSetWindow (parent);
    glGetIntegerv (GL_VIEWPORT, view);
    cbutton->wP = (float)w/view[2], cbutton->hP = (float)h/view[3];
    cbutton->xP = (float)x/view[2], cbutton->yP = (float)y/view[3];
    glutSetWindow (cbutton->window);
    return cbutton->window;
}
