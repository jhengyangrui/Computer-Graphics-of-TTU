//
// covered transparent floor
// mutiple texture (duck, 2x); 2x toggle; turn
// and single texture (aim)
// left for you ... motion, integration
//
#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"

#pragma comment (lib, "glew32.lib")

int mainwin, viewer;
int flip = 1, tag = 1, die = 0, c=0, temp = 0;
float xpos = 0;
float angle = 0;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}
void grid()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	// turn both texture units off
	glActiveTexture (GL_TEXTURE0); glDisable (GL_TEXTURE_2D);
	glActiveTexture (GL_TEXTURE1); glDisable (GL_TEXTURE_2D);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1.3, 1.3);
	glColor4f (.5,0,.5, 0.2);
	glBegin (GL_QUADS);
	glVertex3i (-10,0,10), glVertex3i (10,0,10), glVertex3i (10,0,-10), glVertex3i (-10,0,-10);
	glEnd();
	glDisable (GL_POLYGON_OFFSET_FILL);

	glColor3f (1,1,1);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10,0,i); glVertex3i (10,0,i);
		glVertex3i (i,0,-10); glVertex3i (i,0,10);
	}
	glEnd();
	glPopAttrib();
}

GLuint aimid, duckid, doubleid;



void aim () // use texture0
{
	int tex2dbound;  // of texture0
	glGetIntegerv (GL_TEXTURE_BINDING_2D, &tex2dbound);  // save
 
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glActiveTexture (GL_TEXTURE1); glDisable (GL_TEXTURE_2D);
	glActiveTexture (GL_TEXTURE0);

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (-1,-1);
	glBindTexture (GL_TEXTURE_2D, aimid);
	glBegin (GL_QUADS);
	  glTexCoord2f (0,0); glVertex2f (-.2+xpos,0);
	  glTexCoord2f (1,0); glVertex2f (+.2+xpos,0);
	  glTexCoord2f (1,1); glVertex2f (+.2+xpos, .4);
	  glTexCoord2f (0,1); glVertex2f (-.2+xpos, .4);	  
	glEnd();

	glPopAttrib();

	glBindTexture (GL_TEXTURE_2D, tex2dbound);  // restore
}

void duck()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0);

	glBegin (GL_QUADS);

	glMultiTexCoord2f (GL_TEXTURE0, 0,0); glMultiTexCoord2f (GL_TEXTURE1, 0,0); glVertex2f (-1+xpos,0);
	glMultiTexCoord2f (GL_TEXTURE0, flip*1,0); glMultiTexCoord2f (GL_TEXTURE1, 1,0); glVertex2f (1+xpos,0);
	glMultiTexCoord2f (GL_TEXTURE0, flip*1,1); glMultiTexCoord2f (GL_TEXTURE1, 1,1); glVertex2f (1+xpos,2);
	glMultiTexCoord2f (GL_TEXTURE0, 0,1); glMultiTexCoord2f (GL_TEXTURE1, 0,1); glVertex2f (-1+xpos,2);
	
	glEnd();

	glPopAttrib();
}
void Status () {
	if (-1+xpos > 5){
		flip = flip*(-1);
		tag = 0;
	}
	if (-1+xpos < -5){
		flip = flip*(-1);
		tag = 1;
	}
}


void timer (int dummy)
{
    if(die == 1) {
	    angle = angle - 45;
		if(angle == -90) {		
		    die = 0;
			temp = 1;
		}
	}	
	c = c + temp;
	if(c == 5) {
		angle = angle + 45;
		temp = 0;
		if(angle == 0)
			c = 0;
	}
	
	if (tag == 1) {
	    xpos = xpos+0.1;
		Status();
	}
	else {
		xpos = xpos-0.1;	
		Status();		
	}	


	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}
void content() 
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	grid();	

	glPushMatrix();

	glRotatef(angle,1,0,0);

	duck();

	aim();

	glPopMatrix();

	glutSwapBuffers();
}

void init()
{
	glClearColor (.4,.4,.4,1);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHT0);

	glewInit();

	pngSetStandardOrientation (1);
//	pngSetStencil (255,255,255);
	pngSetStencil (0,0,0);

	duckid = pngBind("duck.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);  // rgba
	doubleid = pngBind("2x.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR); 
	aimid = pngBind("aim.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR); 
//	aimid = pngBind("aim.png", PNG_NOMIPMAP, PNG_STENCIL, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR); 

	glActiveTexture (GL_TEXTURE0);
	glEnable (GL_TEXTURE_2D); 
	glBindTexture (GL_TEXTURE_2D, duckid);

	glActiveTexture (GL_TEXTURE1);
	glEnable (GL_TEXTURE_2D); 
	glBindTexture (GL_TEXTURE_2D, doubleid);

}

void keyboard (unsigned char key, int x, int y)
{
	static int toggle_2x = 1;
	if (key == 'b') {
		toggle_2x ^= 1;  // 0 <--> 1
	}
	if (key == ' ') {
		die = 1;
	}

	if (toggle_2x) {
		glActiveTexture (GL_TEXTURE1);
		glEnable (GL_TEXTURE_2D); 
	} else {
		glActiveTexture (GL_TEXTURE1);
		glDisable (GL_TEXTURE_2D); 
	}

	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	glutKeyboardFunc (keyboard);
	glutTimerFunc (100, timer, 0);

	glutMainLoop();
}
