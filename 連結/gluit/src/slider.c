#include <stdio.h>
#include "gluit.h"

typedef struct _slider {
   int window;
   int parentwin;
   int x, y, w, h;
   float xP, yP, wP, hP;
   
   GLubyte rgb[3];  // color of knob
   
   void (*update) (float); 
   float percentage; // where the knob is, [0,1]
   struct _slider *next;
   char varname[5];
} Slider;

//#define NULL 0
static Slider* _sliders = NULL;
static Slider* _sliderHit = NULL;

static Slider*
_sliderCurrent (void)
{
    Slider *slider = _sliders;
    int window = glutGetWindow ();
    
    while (slider) {
    	if (slider->window == window)
    	   break;
    	slider = slider->next;
    }

    if (! slider)
       printf ("error...\n");

    return slider;
}

#define OFFSET 10

static void
_sliderMotion (int x, int y)
{
    float p;
	int w = _sliderHit->w;
    
    p = (120.*x/w - OFFSET)/100;
    if (p > 1.0) p = 1.0;
    else if (p < 0.0) p = 0.0;
    _sliderHit->percentage = p;


    glutPostRedisplay();

	_sliderHit->update (p);
}

#define Max(x,y) ((x) > (y) ? (x): (y))
#define Min(x,y) ((x) > (y) ? (y): (x))

static void
_sliderMouse (int button, int state, int x, int y)
{
    float p;
    
    Slider *slider = _sliderCurrent();
    int w = slider->w;

    _sliderHit = slider;
        
    switch (button) {
   	  case GLUT_LEFT_BUTTON:
   	  if (state == GLUT_DOWN) {
   	      p = (120.*x/w - OFFSET)/100;
   	      if (p > 1.0) p = 1.0;
   	      else if (p < 0.0) p = 0.0;
   	      slider->percentage = p;

	      glutSetWindow (slider->window);
		  glutPostRedisplay();
		  slider->update (p);
	  }
   	  break;
   }

}


static void 
_sliderDisplay (void)
{
    int move;
    
    Slider *slider = _sliderCurrent();
    glViewport (0,0,slider->w,slider->h);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (0, 120, -15, 15);
    glMatrixMode (GL_MODELVIEW); glPushMatrix (); glLoadIdentity();
    
	setfont ("helvetica",12);
	drawstr (1,0, slider->varname);

    glPushAttrib (GL_ALL_ATTRIB_BITS);
    glDisable (GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);
	// the track
	glLineWidth (3.0);
	glBegin (GL_LINES);
	   glColor3ub (255,255,255);
	   glVertex2i (0+OFFSET, 0);
	   glVertex2i (120-OFFSET, 0);
	glEnd(); 
	
	// border
	glLineWidth (1.0);
	glBegin (GL_LINE_LOOP);
	   glColor3ub (255,255,255);
	   glVertex2i (0, -15); glVertex2i (120, -15); glVertex2i (120, 15);glVertex2i (0, 15);
	glEnd();

	// the knob
	glBegin (GL_QUADS);
	   glColor3ub (slider->rgb[0], slider->rgb[1], slider->rgb[2]);
       move = 100*slider->percentage + OFFSET;
	   glVertex2i (move+5, 5);
	   glVertex2i (move-5, 5);
	   glVertex2i (move-5,-5);
	   glVertex2i (move+5,-5);
	glEnd();

    glPopMatrix ();
    glMatrixMode (GL_PROJECTION); glPopMatrix();
    glMatrixMode (GL_MODELVIEW);

   glPopAttrib();
   glutSwapBuffers();
}

static Slider*
_sliderSelect (int win)
{
    Slider *slider = _sliders;
    
    while (slider) {
    	if (slider->window == win)
    	   break;
    	slider = slider->next;
    }

    if (! slider)
       printf ("error...\n");

    return slider;
}
    
void SliderReshape (int win)
{
    Slider *slider = _sliderSelect (win);  // get the viewer correspond to win (id)
    int view[4];  
    int parentwin = slider->parentwin;

	glutSetWindow (parentwin);
	glGetIntegerv (GL_VIEWPORT, view);

	//
	// reposition in the parent
	glutSetWindow (win);

	slider->x = view[2]*slider->xP;
	slider->y = view[3]*slider->yP;
	slider->w = view[2]*slider->wP;
	slider->h = view[3]*slider->hP;

 	if (slider->w && slider->h) {// w & h become 0 when window is minimized
	glutPositionWindow (slider->x, slider->y);
	glutReshapeWindow (slider->w, slider->h);
	glutPostRedisplay ();
	}
}

int SliderNew (int parent, int x, int y, int w, int h, GLubyte *rgb, float percent, void *update(float), char* varname)
{
    Slider *slider = (Slider*) malloc (sizeof (Slider));
    int view[4];
    
    slider->next = _sliders;
    _sliders = slider;
    
    slider->window = glutCreateSubWindow (parent, x, y, w, h);
    slider->parentwin = parent;
    slider->x = x; slider->y = y;
    slider->w = w; slider->h = h;
    slider->rgb[0] = rgb[0]; slider->rgb[1] = rgb[1]; slider->rgb[2] = rgb[2]; 
    
    slider->percentage = percent;
    slider->update = (void*) update;
    
    glutDisplayFunc (_sliderDisplay);
    glutMouseFunc (_sliderMouse);
    glutMotionFunc (_sliderMotion);
	
	glutSetCursor (GLUT_CURSOR_CROSSHAIR);
	
	
    glutSetWindow (parent);
	glGetIntegerv (GL_VIEWPORT, view);
	slider->wP = (float)w/view[2], slider->hP = (float)h/view[3];
	slider->xP = (float)x/view[2], slider->yP = (float)y/view[3];
	strcpy (slider->varname, varname);
	glutSetWindow (slider->window);    
    return slider->window;
}
