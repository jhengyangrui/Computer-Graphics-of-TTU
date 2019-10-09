#include <stdio.h>
#include <stdarg.h>
#include "gluit.h"

static GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;

//
// valid combinations: 
//  "helvetica" [10], 12, 18  []: default (set size = any)
//  "times roman" [10], 24
//  "8x13", "9x15"
//
void
setfont(char* name, int size)
{
    if (strcmp(name, "helvetica") == 0) {
        font_style = GLUT_BITMAP_HELVETICA_10;
		if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}


//
// usage: Drawstr (x, y, z, "hello world");
//        Drawstr (x, y, z, "answer is %d", ans);
//
void 
Drawstr(GLfloat x, GLfloat y, GLfloat z, char* format, ...)
{
    va_list args;    // variable arguments; require <stdarg.h>
    char buffer[255], *s;
    
    // variable argument stuff
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    glRasterPos3f(x, y, z);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

//
// plotstr: vector font
//
// the size is quite big (need about [-500,500]x[-500,500]
//

void 
Plotstr(GLfloat x, GLfloat y, GLfloat z, GLfloat size, char *format, ...)
{
    va_list args;    // variable arguments; require <stdarg.h>
    char buffer[255], *s;
    
    // variable argument stuff
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef (size,size,size);

  for (s = buffer; *s; s++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
  glPopMatrix();
}



//
// usage: drawstr (x, y, "hello world");
//        drawstr (x, y, "answer is %d", ans);
//
void 
drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;    // variable arguments; require <stdarg.h>
    char buffer[255], *s;
    
    // variable argument stuff
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

//
// plotstr: vector font
//
// the size is quite big (need about [-500,500]x[-500,500]
//

void 
plotstr(GLfloat x, GLfloat y, char *format, ...)
{
    va_list args;    // variable arguments; require <stdarg.h>
    char buffer[255], *s;
    
    // variable argument stuff
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

  glPushMatrix();
  glTranslatef(x, y, 0);

  for (s = buffer; *s; s++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
  glPopMatrix();
}

