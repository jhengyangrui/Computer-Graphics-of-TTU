#include <stdarg.h>
#include <gl/glut.h>
#include <string.h>
//#include "gluit.h"
//
// variable argument
//
extern void drawstr (GLuint, GLuint, char*, ...);
void help_screen(char* first, ...)
{
    int startline;

    char* str;
	va_list marker;

	glPushAttrib (GL_ALL_ATTRIB_BITS);
 
	glDisable (GL_LIGHTING);   // THIS IS IMPORTANT; for glColor to be effective
        glDisable (GL_TEXTURE_2D);
        glDisable (GL_TEXTURE_1D);
    glDisable (GL_COLOR_MATERIAL);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	 		
	glDisable (GL_CULL_FACE);

	glDisable (GL_DEPTH_TEST);
	glMatrixMode (GL_PROJECTION); 
	glPushMatrix (); glLoadIdentity(); gluOrtho2D (0,12,0,12);
	glMatrixMode (GL_MODELVIEW);  
	glPushMatrix (); glLoadIdentity ();   // resetting the MODELVIEW matrix

	glColor4ub (255, 0, 0, 100);
	glRecti (1,1,11,11);
   
	glColor4ub (255, 255, 0, 100);
	
	startline = 10;

	// variable argument stuff
    str = first;
	va_start (marker, first);
	while (strcmp ("fff", str)) {
        drawstr (2, startline--, str);
        str = va_arg (marker, char*);
	}
	va_end (marker);

	glPopMatrix ();   // restoring MODELVIEW
	glMatrixMode (GL_PROJECTION); 	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);

	glPopAttrib();
}
