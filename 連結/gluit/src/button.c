#include <stdio.h>
#include "gluit.h"

typedef struct _button {
    int window;
    int parentwin;
    int x,y,w,h;
    float xP,yP,wP,hP;
    
    GLubyte rgb[3];  // color
    char text[30];   // at most 30 characters: baloon help
	char on_text[10];  // on-button annotation
    void (*action)(void);
    struct _button *next;
} Button;

//#define NULL 0
static Button* _buttons = NULL;
static Button* _buttonHit = NULL;

#define BUTTON_RELEASE 1
#define BUTTON_PRESSED 2

static void 
_buttonInit()
{
    int p1[2], p2[2], p3[2], p4[2], p5[2], p6[2], p7[2], p8[2];
#define PAD 10

	p1[0] = 0, p1[1] = 0;
	p2[0] = 100, p2[1] = 0;
	p3[0] = 100, p3[1] = 50;
	p4[0] = 0, p4[1] = 50;

	p5[0] = p1[0]+PAD, p5[1] = p1[1]+PAD;
	p6[0] = p2[0]-PAD, p6[1] = p2[1]+PAD;
	p7[0] = p3[0]-PAD, p7[1] = p3[1]-PAD;
	p8[0] = p4[0]+PAD, p8[1] = p4[1]-PAD;

	glNewList (BUTTON_RELEASE, GL_COMPILE);
	glBegin (GL_QUADS);
	    glColor3ub (255,255,255);
	    glVertex2iv (p1); glVertex2iv (p5);glVertex2iv (p8);glVertex2iv (p4);
        glVertex2iv (p8); glVertex2iv (p7);glVertex2iv (p3);glVertex2iv (p4);
 
	    glColor3ub (200,200,200);
		glVertex2iv (p5); glVertex2iv (p6);glVertex2iv (p7);glVertex2iv (p8);
	    
		glColor3ub (150,150,150);
		glVertex2iv (p1); glVertex2iv (p2);glVertex2iv (p6);glVertex2iv (p5);
        glVertex2iv (p2); glVertex2iv (p3);glVertex2iv (p7);glVertex2iv (p6);

	glEnd();
	glEndList();

    glNewList (BUTTON_PRESSED, GL_COMPILE);
	glBegin (GL_QUADS);
	    glColor3ub (150,150,150);
	    glVertex2iv (p1); glVertex2iv (p5);glVertex2iv (p8);glVertex2iv (p4);
        glVertex2iv (p8); glVertex2iv (p7);glVertex2iv (p3);glVertex2iv (p4);
 
	    glColor3ub (200,200,200);
		glVertex2iv (p5); glVertex2iv (p6);glVertex2iv (p7);glVertex2iv (p8);
	    
		glColor3ub (255,255,255);
		glVertex2iv (p1); glVertex2iv (p2);glVertex2iv (p6);glVertex2iv (p5);
        glVertex2iv (p2); glVertex2iv (p3);glVertex2iv (p7);glVertex2iv (p6);

	glEnd();
	glEndList();
}

static Button* 
_buttonCurrent (void)
{
    Button *button = _buttons;
    
    int window = glutGetWindow ();
    
    while (button) {
    	if (button->window == window)
    	    break;
    	button = button->next;
    }
    if (! button)
        printf ("error ...\n");
    return button;
}

static int mode = BUTTON_RELEASE;

static void
_buttonMouse (int button, int state, int x, int y)
{
    Button *thebutton = _buttonCurrent();
    
    _buttonHit = thebutton;
    switch (button) {
	case GLUT_LEFT_BUTTON:
    	if (state == GLUT_DOWN) {
			mode = BUTTON_PRESSED;
			thebutton->action ();   // execute 
        } else if (state == GLUT_UP) {
			mode = BUTTON_RELEASE;
		}
		glutSetWindow (thebutton->window);
		glutPostRedisplay ();
		break;
	}
}

#if 0
// implementing "balloon help"
// entry does not work
static void
_buttonPassive (int x, int y)
{
    Button *button = _buttonCurrent();
//printf ("in button passive\n"); 
    // go back to parent
    // write a blended msg at where the button is
    //
	if (strlen(button->text))
		printf ("%s\n", button->text);  // write to stdout for now
}
#endif

static void 
_buttonDisplay (void)
{
    Button *button = _buttonCurrent();
	GLubyte *rgb = button->rgb;

	glViewport (0,0,button->w, button->h);
    
	glClear (GL_COLOR_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (0,100,0,50);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	
    glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (mode == BUTTON_RELEASE) 
		glCallList (BUTTON_RELEASE);
	else
		glCallList (BUTTON_PRESSED);
    
	glColor4f (rgb[0]/255.,rgb[1]/255.,rgb[2]/255.,0.5);
	glRecti (0,0,100,100);

	glDisable (GL_BLEND);
	glColor3ub (0,0,0);
	drawstr (25,20, button->on_text);
	glutSwapBuffers();

	glutSetWindow (button->parentwin);   // for idling to continue;
}


static Button*
_buttonSelect (int win)
{
    Button *button = _buttons;
    
    while (button) {
    	if (button->window == win)
    	   break;
    	button = button->next;
    }

    if (! button)
       printf ("error...\n");

    return button;
}
    
void ButtonReshape (int win)
{
    Button *button = _buttonSelect (win);  // get the viewer correspond to win (id)
    int view[4];  
    int parentwin = button->parentwin;

	glutSetWindow (parentwin);
	glGetIntegerv (GL_VIEWPORT, view);

	//
	// reposition in the parent
	glutSetWindow (win);

	button->x = view[2]*button->xP;
	button->y = view[3]*button->yP;
	button->w = view[2]*button->wP;
	button->h = view[3]*button->hP;

	if (button->w && button->h) {// w & h become 0 when window is minimized
	glutPositionWindow (button->x, button->y);
	glutReshapeWindow (button->w, button->h);
	glutPostRedisplay ();
	}
}


int ButtonNew (
int parent, int x, int y, int w, int h, 
GLubyte *rgb, 
char *text, void *action(void), char* on_text)
{
    int view[4];
    Button *button = (Button*) malloc (sizeof (Button));
    button->next = _buttons;
    _buttons = button;
    
    button->window = glutCreateSubWindow (parent, x,y,w,h);
    button->parentwin = parent;
    button->x = x; button->y = y;
    button->w = w; button->h = h;
	button->rgb[0] = rgb[0], button->rgb[1] = rgb[1], button->rgb[2] = rgb[2];
	
	strcpy (button->text, text);
	strcpy (button->on_text, on_text);
    _buttonInit();
    
    button->action = (void*)action;
    glutDisplayFunc (_buttonDisplay);
    glutMouseFunc (_buttonMouse);
//	glutPassiveMotionFunc (_buttonPassive);
    glutSetCursor (GLUT_CURSOR_CROSSHAIR);
    
    glutSetWindow (parent);
    glGetIntegerv (GL_VIEWPORT, view);
    button->wP = (float)w/view[2], button->hP = (float)h/view[3];
    button->xP = (float)x/view[2], button->yP = (float)y/view[3];
    glutSetWindow (button->window);
    return button->window;
}
