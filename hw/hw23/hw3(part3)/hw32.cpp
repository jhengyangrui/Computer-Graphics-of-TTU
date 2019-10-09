#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"
#include <svl/svl.h>

#pragma comment (lib, "glew32.lib")

GLMmodel *al;

int mainwin, viewer;
int rw=400,rh=400;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}


GLuint floortex;

void floor()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glBindTexture (GL_TEXTURE_2D, floortex);
	glEnable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);

	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex3i (-10,0,10); 
	glTexCoord2f (10,0); glVertex3i (10,0,10);
	glTexCoord2f (10,10); glVertex3i (10,0,-10); 
	glTexCoord2f (0,10); glVertex3i (-10,0,-10);
	glEnd();
	glPopAttrib();
}

void cone (float base, float height, float x, float z)
{
	glPushMatrix();
	glTranslatef (x,0,z);
	glRotatef (-90,1,0,0);
	glutSolidCone (base, height, 20,20);
	glPopMatrix();
}

void scene()
{
	floor();

	float red[] = {1,0,0,1};
	float yellow[] = {.8,.8,0,1};
	float purple[] = {.4,0,.4,1};
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	cone (.4,1.2, -2,3);
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	cone (.4,1.6, 2,-2);
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
	cone (.4,1.2, -2.4,-2.5);
}

double theta;
//----------------------------------------------------------------------
GLuint fbo;		// Our handle to the FBO
GLuint depthBuffer;	// Our handle to the depth render buffer
GLuint img;		// Our handle to a texture
int width = 400; 
int height =400;

void initFBO() {
   // Setup our FBO
   glGenFramebuffersEXT(1, &fbo);

   // bind for set up
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

   // Now setup a texture to render to
   glGenTextures(1, &img);
   glBindTexture(GL_TEXTURE_2D, img);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // And attach it to the FBO so we can render to it
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img, 0);
   // Create the render buffer for depth	
   glGenRenderbuffersEXT(1, &depthBuffer);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT, width, height);

   // Attach the depth render buffer to FBO as it's depth attachment
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	
   // Checking 
   GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
      exit(1);	
   // Unbind the FBO for now
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	
}

void ShutDown(GLvoid)
{
    glDeleteFramebuffersEXT(1, &fbo);
    glDeleteRenderbuffersEXT(1, &depthBuffer);
    glDeleteTextures(1,&img);
}

//----------------------------------------------------------------------
GLuint fbo2;		// Our handle to the FBO
GLuint depthBuffer2;	// Our handle to the depth render buffer
GLuint img2;		// Our handle to a texture
int width2 = 400; 
int height2 =400;

void initFBO2() {
   // Setup our FBO
   glGenFramebuffersEXT(1, &fbo2);

   // bind for set up
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);

   // Now setup a texture to render to
   glGenTextures(1, &img2);
   glBindTexture(GL_TEXTURE_2D, img2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // And attach it to the FBO so we can render to it
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img2, 0);
   // Create the render buffer for depth	
   glGenRenderbuffersEXT(1, &depthBuffer2);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer2);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT, width2, height2);

   // Attach the depth render buffer to FBO as it's depth attachment
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer2);
	
   // Checking 
   GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
      exit(1);	
   // Unbind the FBO for now
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	
}

void ShutDown2(GLvoid)
{
    glDeleteFramebuffersEXT(1, &fbo2);
    glDeleteRenderbuffersEXT(1, &depthBuffer2);
    glDeleteTextures(1,&img2);
}

//----------------------------------------------------------------------
GLuint prog;
GLuint p;
float radius = 50.0;
float winxy[2] = {100.,100.};
GLuint centerloc;
GLuint radiusloc;
#if 1
//fbo>場景>fbo2>用img畫灰階>畫Al>用img2畫馬賽克
void content()
{
	glUseProgram(0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);//fbo1
	glDisable (GL_TEXTURE_2D);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	scene();//scene

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, img);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);//fbo clear again
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);//fbo2
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glDepthMask(GL_FALSE);

	glUseProgram(prog);

	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (-1,1,-1,1);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();	

	glEnable (GL_TEXTURE_2D);
	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex2i (-1,-1);
	glTexCoord2f (1,0); glVertex2i (1,-1);
	glTexCoord2f (1,1); glVertex2i (1,1);
	glTexCoord2f (0,1); glVertex2i (-1,1);
	glEnd();

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);

	scene();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glUseProgram(0);


	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_TEXTURE_2D);

	glPushMatrix();
	float r = 1.5;
	glTranslatef (r*cos(theta*vl_pi/180), 0, -r*sin(theta*vl_pi/180));
	glRotatef (theta+180, 0,1,0);
	glTranslatef (0,1,0);
	glmDraw (al, GLM_MATERIAL|GLM_SMOOTH);

	// compute head window coordinate
	// Al's head is about (0,1,0) world coordinate
	// NDC = proj ( [PJ][MV](head) )
	double winx, winy, winz;
	
	double mv[16], proj[16];
	int viewport[4];
	glGetDoublev (GL_MODELVIEW_MATRIX, mv);
	glGetDoublev (GL_PROJECTION_MATRIX, proj);
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluProject (0,0.9,0, mv, proj, viewport, &winx, &winy, &winz);

	// draw something overhead
	/*glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_DEPTH_TEST); glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (0,rw,0,rh);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	
	glPointSize (30);
	glBegin (GL_POINTS);
	glVertex2d (winx, winy);
	glEnd();*/

	//glPopMatrix();

	//glMatrixMode (GL_PROJECTION); glPopMatrix();
	//glMatrixMode (GL_MODELVIEW); glPopMatrix();
	glPopAttrib();
	
	glPopAttrib();
//--------------------------------------------------------------------
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, img2);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);//fbo2 clear again
	
	//glDepthMask(GL_FALSE);

	glUseProgram(p);
	glUniform1i (glGetUniformLocation (p, "tex"), 0);

	GLuint granloc = glGetUniformLocation (p, "gran");
	glUniform1f (granloc, 40.0);

	centerloc = glGetUniformLocation (p, "center");
	glUniform2f (centerloc, winx,winy);
	radiusloc = glGetUniformLocation (p, "radius");
	glUniform1f (radiusloc, radius/2);

	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (-1,1,-1,1);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();	

	glEnable (GL_TEXTURE_2D);
	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex2i (-1,-1);
	glTexCoord2f (1,0); glVertex2i (1,-1);
	glTexCoord2f (1,1); glVertex2i (1,1);
	glTexCoord2f (0,1); glVertex2i (-1,1);
	glEnd();

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();
	glutSwapBuffers();
}
#endif
#if 0
void content()
{
	/*glUseProgram(0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glDisable (GL_TEXTURE_2D);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	scene();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, img);

	//glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);//fbo clear again

	glDepthMask(GL_FALSE);

	glUseProgram(prog);

	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (-1,1,-1,1);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();	

	glEnable (GL_TEXTURE_2D);
	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex2i (-1,-1);
	glTexCoord2f (1,0); glVertex2i (1,-1);
	glTexCoord2f (1,1); glVertex2i (1,1);
	glTexCoord2f (0,1); glVertex2i (-1,1);
	glEnd();

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);

	scene();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glUseProgram(0);


	//glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_TEXTURE_2D);

	glPushMatrix();
	float r = 1.5;
	glTranslatef (r*cos(theta*vl_pi/180), 0, -r*sin(theta*vl_pi/180));
	glRotatef (theta+180, 0,1,0);
	glTranslatef (0,1,0);
	glmDraw (al, GLM_MATERIAL|GLM_SMOOTH);

	// compute head window coordinate
	// Al's head is about (0,1,0) world coordinate
	// NDC = proj ( [PJ][MV](head) )
	double winx, winy, winz;
	
	double mv[16], proj[16];
	int viewport[4];
	glGetDoublev (GL_MODELVIEW_MATRIX, mv);
	glGetDoublev (GL_PROJECTION_MATRIX, proj);
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluProject (0,0.9,0, mv, proj, viewport, &winx, &winy, &winz);

	// draw something overhead
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_DEPTH_TEST); glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (0,rw,0,rh);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	
	glPointSize (30);
	glBegin (GL_POINTS);
	glVertex2d (winx, winy);
	glEnd();

	//glPopMatrix();

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();
	glPopAttrib();
	
	glPopAttrib();*/
	

	double winx, winy, winz;
	
	double mv[16], proj[16];
	int viewport[4];

	//glUseProgram(0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glDisable (GL_TEXTURE_2D);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	scene();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, img);

	//glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);//fbo clear again

	glDepthMask(GL_FALSE);

	//glUseProgram(prog);

	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (-1,rw,-1,rh);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();	

	glEnable (GL_TEXTURE_2D);
	/*glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex2i (-1,-1);
	glTexCoord2f (1,0); glVertex2i (1,-1);
	glTexCoord2f (1,1); glVertex2i (1,1);
	glTexCoord2f (0,1); glVertex2i (-1,1);
	glEnd();*/

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);

	scene();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glUseProgram(0);


	//glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_TEXTURE_2D);


	/*glGetDoublev (GL_MODELVIEW_MATRIX, mv);
	glGetDoublev (GL_PROJECTION_MATRIX, proj);
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluProject (0,0.9,0, mv, proj, viewport, &winx, &winy, &winz);*/

	glPushMatrix();
	float r = 1.5;
	glTranslatef (r*cos(theta*vl_pi/180), 0, -r*sin(theta*vl_pi/180));
	glRotatef (theta+180, 0,1,0);
	glTranslatef (0,1,0);
	glUseProgram(0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glmDraw (al, GLM_MATERIAL|GLM_SMOOTH);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glGetDoublev (GL_MODELVIEW_MATRIX, mv);
	glGetDoublev (GL_PROJECTION_MATRIX, proj);
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluProject (0,0.9,0, mv, proj, viewport, &winx, &winy, &winz);

	// Second pass
	glUseProgram(p); 
	glUniform2f (centerloc, winx,winy);//winxy[0], winxy[1]);
	glUniform1f (radiusloc, radius/2);

	//glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION); glPushMatrix();
	glLoadIdentity(); gluOrtho2D (-1.0,1.0,-1.0,1.0);
	glMatrixMode (GL_MODELVIEW); glPushMatrix();
	glLoadIdentity();
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, img);
	glBegin (GL_QUADS);
	glTexCoord2i(0,0); glVertex2i (-1,-1);
	glTexCoord2i(1,0); glVertex2i (1,-1);
	glTexCoord2i(1,1); glVertex2i (1,1);
	glTexCoord2i(0,1); glVertex2i (-1,1);
	glEnd();
	glDisable (GL_TEXTURE_2D);
	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();

	// compute head window coordinate
	// Al's head is about (0,1,0) world coordinate
	// NDC = proj ( [PJ][MV](head) )



	// draw something overhead
	/*glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_DEPTH_TEST); glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D (0,rw,0,rh);  
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	
	glPointSize (30);
	glBegin (GL_POINTS);
	glVertex2d (winx, winy);
	glEnd();



	//glPopMatrix();

	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();*/
	glPopAttrib();
	
	glPopAttrib();
//--------------------------------------------------
	//glUseProgram(0);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	//glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glutSolidTeapot(1.0);
	//glTranslatef (r*cos(theta*vl_pi/180), 0, -r*sin(theta*vl_pi/180));
	//glRotatef (theta+180, 0,1,0);
	//glTranslatef (0,1,0);
	//glmDraw (al, GLM_MATERIAL|GLM_SMOOTH);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// Second pass
	//glUseProgram(p); 
	//glUniform2f (centerloc, winx,winy);//winxy[0], winxy[1]);
	//glUniform1f (radiusloc, radius);

	//glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glMatrixMode (GL_PROJECTION); glPushMatrix();
	//glLoadIdentity(); gluOrtho2D (-1.2,1.2,-1.2,1.2);
	//glMatrixMode (GL_MODELVIEW); glPushMatrix();
	//glLoadIdentity();
	//glEnable (GL_TEXTURE_2D);
	//glBindTexture (GL_TEXTURE_2D, img);
	//glBegin (GL_QUADS);
	//glTexCoord2i(0,0); glVertex2i (-1,-1);
	//glTexCoord2i(1,0); glVertex2i (1,-1);
	//glTexCoord2i(1,1); glVertex2i (1,1);
	//glTexCoord2i(0,1); glVertex2i (-1,1);
	//glEnd();
	//glDisable (GL_TEXTURE_2D);
	//glMatrixMode (GL_PROJECTION); glPopMatrix();
	//glMatrixMode (GL_MODELVIEW); glPopMatrix();
//----------------------------------------------------------------
	glutSwapBuffers();
}
#endif

int turn;
void timer (int dummy)
{
	glutTimerFunc (10, timer, 0);
	glutPostRedisplay();
	if (turn) theta += 2;
}

void init()
{
	glewInit();
	glClearColor (.4,.4,.4,1);
	glEnable (GL_DEPTH_TEST);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	
	al = glmReadOBJ ("al.obj");
	glmUnitize (al);
	glmFacetNormals (al);
	glmVertexNormals (al, 90.0);
	
	floortex = pngBind("marble.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	
	initFBO();
	initFBO2();
	extern GLuint setShaders (char*, char*);
	prog = setShaders ("grey.vert", "grey.frag");
//------------------------------------------------------------------------------------------
	p = setShaders ("mosaic.vert", "mosaic.frag");

	/*glUseProgram(p);
	glUniform1i (glGetUniformLocation (p, "tex"), 0);

	GLuint granloc = glGetUniformLocation (p, "gran");
	glUniform1f (granloc, 40.0);

	centerloc = glGetUniformLocation (p, "center");
	glUniform2f (centerloc, winx,winy);
	radiusloc = glGetUniformLocation (p, "radius");
	glUniform1f (radiusloc, radius);*/
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == ' ')
		turn ^= 1;
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("Part I");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	ViewerExtent (viewer, 8.0);
	glutTimerFunc (10, timer, 0);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}
