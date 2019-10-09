#ifndef __gluit_h
#define __gluit_h


#include <stdlib.h>
#include <string.h>

#include "glm.h"

#include "tga.h"
#include "stlload.h"

#include "macros.h"
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifdef _MSC_VER
	#pragma comment (lib, "gluit.lib")
#endif

// fonts.c
void setfont (char*, int);
void Drawstr (GLfloat, GLfloat, GLfloat, char*, ...);
void Plotstr (GLfloat, GLfloat, GLfloat, GLfloat, char*, ...);
void drawstr (GLuint, GLuint, char*, ...);
void plotstr (GLfloat, GLfloat, char*, ...);

// help.c
void help_screen (char*, ...);

// viewer.c, slider.c, button.c
extern void ViewerRedisplay (int);
extern void ViewerReshape (int);
extern int ViewerNew (int, int, int, int, int, void (*content)(void));
extern void ViewerMouseMotion (int, void*, void*);
extern void ViewerAddClicker (int, void (*content)(int,int,int,int));
extern void ViewerFOVY (int, float);
extern void ViewerCenter (int, float, float, float);
extern void ViewerExtent (int, float);
extern void SliderReshape (int);
#ifdef __cplusplus
extern int SliderNew (int, int, int, int, int, GLubyte*, float, void *, char* = "");
#else
extern int SliderNew (int, int, int, int, int, GLubyte*, float, void *, char*);
#endif
extern void ButtonReshape (int);
#ifdef __cplusplus
extern int ButtonNew (int, int, int, int, int, GLubyte*, char *, void *, char* = "");
#else
extern int ButtonNew (int, int, int, int, int, GLubyte*, char *, void *, char*);
#endif

extern int CheckButtonNew (int, int, int, int, int, char*, GLboolean*, int);
extern void CheckButtonReshape (int);


//#ifdef __cplusplus
//}
//#endif
#endif 