#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "gluit.h"

#define M_PI 3.14159265
#define RAD(x) (((x)*M_PI)/180.)

//#define NULL 0
#define ZOOM 4 
#define ROT  2

// viewer.c
// implements a subwindow of spherical viewer
// where objects are placed around origin
//
// follow the style of bounce.exe (Nate Robins)
//
// no nested viewer allowed
//
//
typedef struct _viewer {
    int window;   // glut id
    int parentwin;
	int x, y;     // relative pos in parentwin
	int w, h;     // size of viewer
    float xP, yP, wP, hP;  // percentage for reshaping
    float fovy;
    float center[3];
    
    // for viewobj purpose
    float rotY, phi, cylR;
    int ox, oy;
    
    // display content
	void (*content)(void);
    void (*clicker)(int,int,int,int);

	// viewing parameter? ortho/perspective, fovy, ...
    struct _viewer *next;
} Viewer;

static Viewer* _viewers = NULL;      // tail of link lists
static Viewer* _viewerHit = NULL;    // the viewer that got the mouse click

static int mot = 0;

enum {X, Y, Z};

static Viewer *
_viewerCurrent (void)
{
    Viewer *viewer = _viewers;
    int window = glutGetWindow ();
    
    while (viewer) {
    	if (viewer->window == window)
    	   break;
    	viewer = viewer->next;
    }

    if (! viewer)
       printf ("error...\n");

    return viewer;
}

// for the sake of readability (with old code)
static int* ox = 0, *oy=0;

static void
rotate(const int x, const int y) 
{
    float rotY, phi;

	rotY = _viewerHit->rotY;
	phi = _viewerHit->phi;
    
	rotY += x-*ox;   // longitude
    if (rotY > 360.) rotY -= 360.;
    else if (rotY < -360.) rotY += 360.;

	phi += y-*oy;  // altitude [-90, 90]
	if (phi > 90) phi = 90;
	else if (phi < -90) phi = -90;

    *ox = x; *oy = y;
	_viewerHit->rotY = rotY;
	_viewerHit->phi = phi;

	glutSetWindow (_viewerHit->window);
    glutPostRedisplay();
}

static void zoom (const int x, const int y) 
{
#define more(x,y) ((x) > (y)? (x) : (y))
	float cylR, minR = 1.0;

	cylR = _viewerHit->cylR;

	cylR -= (y-*oy)/50.;
	cylR = more (cylR, minR);

    *ox = x; *oy = y;
	_viewerHit->cylR = cylR;

    glutPostRedisplay();
}


static void
_viewerMotion(int x, int y) {
	switch (mot) {
	case ROT:
		rotate(x,y);
		break;
	case ZOOM:
		zoom(x,y);
		break;
	}
}

static void
_viewerMouse(int button, int state, int x, int y) 
{
    Viewer *viewer = _viewerCurrent(); 
    _viewerHit = viewer;   // register _viewerHit only when mouse down

    // process clicker first
	if (viewer->clicker) 
		(viewer->clicker)(button,state,x,y);

    ox = &(_viewerHit->ox);
    oy = &(_viewerHit->oy);
    
    if(state == GLUT_DOWN) {
	    switch(button) {
		case GLUT_LEFT_BUTTON:
		    if (glutGetModifiers() & GLUT_ACTIVE_CTRL) 
			mot = ZOOM;
		    else
			mot = ROT;
		    break;
		case GLUT_MIDDLE_BUTTON:
		case GLUT_RIGHT_BUTTON:
		break;
 	    }
	    _viewerMotion(*ox = x, *oy = y);  // register the starting position 
		                         // when mouse is first down
	} else if (state == GLUT_UP) {
		mot = 0;
    }
}

static Viewer*
_viewerSelect (int win)
{
    Viewer *viewer = _viewers;
    
    while (viewer) {
    	if (viewer->window == win)
    	   break;
    	viewer = viewer->next;
    }

    if (! viewer)
       printf ("error...\n");

    return viewer;
}

static void
_viewerDisplay (void)
{
   float phi, rotY, cylR;

   Viewer *viewer = _viewerCurrent();

   phi = viewer->phi;
   rotY = viewer->rotY; 
   cylR = viewer->cylR;

   glViewport (0,0,viewer->w, viewer->h);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   
   gluPerspective(viewer->fovy, (GLfloat) (viewer->w)/(GLfloat) (viewer->h), 0.1, 2000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(-sin(RAD(rotY))*cos(RAD(phi))*cylR, cylR*sin(RAD(phi)),cos(RAD(rotY))*cos(RAD(phi))*cylR, 0.,0.,0., 0.,1.,0.); 

   glTranslatef (-viewer->center[X], -viewer->center[Y], -viewer->center[Z]);
   
   viewer->content();
}


//=======================================================================
//  The following functions are for client use:
//  1. ViewerRedisplay: respond to redisplay demand from client (animation control)
//  2. ViewerReshape:  respond to reshape of client
//  3. ViewerNew: the main constructor
//  4. ViewerMouseMotion: overwrite the default mouse and motion callback fns
//  5. ViewerFOVY: set perspective FOVY
//  6. ViewerCenter:
//  7. ViewerExtent: 

void ViewerRedisplay (int win)
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
	glutSetWindow (win);
    glutPostRedisplay();
}

void ViewerReshape (int win)
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
    int view[4];  
	int parentwin = viewer->parentwin;

	glutSetWindow (parentwin);
	glGetIntegerv (GL_VIEWPORT, view);

	//
	// reposition in the parent
	glutSetWindow (win);

	viewer->x = view[2]*viewer->xP;
	viewer->y = view[3]*viewer->yP;
	viewer->w = view[2]*viewer->wP;
	viewer->h = view[3]*viewer->hP;
    if (viewer->w && viewer->h) {    // w & h become 0 when window is minimized
		glutPositionWindow (viewer->x, viewer->y);
		glutReshapeWindow (viewer->w, viewer->h);
		glutPostRedisplay ();
	}
}

//
// the content is assumed to be centered at origin
// about unit sphere size.
//
int ViewerNew (int parent, int x, int y, int w, int h, void (*content)(void))
{
    Viewer *viewer = (Viewer*) malloc (sizeof (Viewer));
    int view[4];

    // linked list set up
    viewer->next = _viewers;
    _viewers = viewer;
    
    viewer->window = glutCreateSubWindow (parent, x, y, w, h);
    viewer->parentwin = parent;
    viewer->x = x; viewer->y = y;
    viewer->w = w; viewer->h = h;
   
	viewer->content = content;
    viewer->clicker = 0;

    viewer->center[X] = viewer->center[Y] = viewer->center[Z] = 0;
	viewer->fovy = 65.0;
    viewer->cylR = 5.5;   // default value;
	viewer->phi = viewer->rotY = 0.0;
    
    glutDisplayFunc (_viewerDisplay);
    glutMouseFunc (_viewerMouse);
    glutMotionFunc (_viewerMotion);

   	//
	// also store a percentage so that we can reshape nicely
	//

    glutSetWindow (parent);
	glGetIntegerv (GL_VIEWPORT, view);
	viewer->wP = (float)w/view[2], viewer->hP = (float)h/view[3];
	viewer->xP = (float)x/view[2], viewer->yP = (float)y/view[3];
	glutSetWindow (viewer->window);

	return viewer->window;
}

// overwriting the spherical mouse/motion callback
void ViewerMouseMotion (
	int win, 
	void (*mouse)(int, int, int, int),
	void (*motion)(int,int)
)
{
    glutSetWindow (win);
	glutMouseFunc (mouse);
	glutMotionFunc (motion);
}

void ViewerAddClicker (int win, void (*mouse)(int,int,int,int))
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
    viewer->clicker = mouse;
}

void ViewerFOVY (int win, float fovy)
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
    viewer->fovy = fovy;
	glutSetWindow (win);
    glutPostRedisplay();
}

void ViewerCenter (int win, float x, float y, float z)
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
    viewer->center[X] = x;
	viewer->center[Y] = y;
	viewer->center[Z] = z;
	glutSetWindow (win);
    glutPostRedisplay();
}

void ViewerExtent (int win, float extent)
{
    Viewer *viewer = _viewerSelect (win);  // get the viewer correspond to win (id)
	viewer->cylR = extent;
	glutSetWindow (win);
    glutPostRedisplay();
}
