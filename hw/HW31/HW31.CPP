#include <glut.h>
#include <math.h>
#include "gluit.h"
#include "gl/glpng.h"
#include <svl/svl.h>


float x, z;    // current pos
float current_origin_x, current_origin_z;

float angle;
float dis;
float speed = 1.0;
float maxspeed = 5.0;
GLMmodel *plane;
GLuint skybox;
GLuint Dashboard;
int toggle = 1;
int count = 0;
int wi,he;
int mainwin, viewer;

void reshape(int w, int h)
{
	wi = w;
	he = h;
	/*
	glViewport(0,0,w,h);
	ViewerReshape(viewer);*/
}
void grid (void)
{
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
#define MIN -20
#define MAX 20
	int i;
	glColor3ub (255,255,255);
	glBegin (GL_LINES);
	for (i = MIN; i <= MAX; i++) {
		glVertex3i (i, 0, MIN);	glVertex3i (i, 0, MAX);
		glVertex3i (MIN,0,i);	glVertex3i (MAX,0,i);
	}
	glEnd();
	glPopAttrib();
}
void gridsky (void)
{
	int lenth = 40;
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glBindTexture (GL_TEXTURE_2D, skybox);
	glEnable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);	
	glColor3ub (255,255,255);

	glBegin (GL_QUADS);
	glTexCoord2f (.35,.5); glVertex3i (-lenth,0,lenth);
	glTexCoord2f (.35,.75); glVertex3i (-lenth,0,-lenth); 
	glTexCoord2f (.6,.75); glVertex3i (lenth,0,-lenth); 
	glTexCoord2f (.6,.5); glVertex3i (lenth,0,lenth);
	glEnd();
		
	glBegin (GL_QUADS);
	glTexCoord2f (.35,.25); glVertex3i (-lenth,lenth/2,-lenth); 
	glTexCoord2f (.35,.5); glVertex3i (-lenth,0,-lenth);
	glTexCoord2f (.6,.5); glVertex3i (lenth,0,-lenth);
	glTexCoord2f (.6,.25); glVertex3i (lenth,lenth/2,-lenth); 
	glEnd();
		
	glBegin (GL_QUADS);
	glTexCoord2f (0,.25); glVertex3i (-lenth,lenth/2,lenth);
	glTexCoord2f (0,.5); glVertex3i (-lenth,0,lenth); 
	glTexCoord2f (.35,.5); glVertex3i (-lenth,0,-lenth); 
	glTexCoord2f (.35,.25); glVertex3i (-lenth,lenth/2,-lenth); 
	glEnd();
		
	glBegin (GL_QUADS);
	glTexCoord2f (.7,.25); glVertex3i (lenth,lenth/2,lenth);
	glTexCoord2f (.7,.5); glVertex3i (lenth,0,lenth); 
	glTexCoord2f (1,.5); glVertex3i (lenth,0,-lenth); 
	glTexCoord2f (1,.25); glVertex3i (lenth,lenth/2,-lenth);
	glEnd();
		
	glBegin (GL_QUADS);
	glTexCoord2f (.65,1); glVertex3i (lenth,lenth/2,lenth);
	glTexCoord2f (.65,.75); glVertex3i (lenth,0,lenth); 
	glTexCoord2f (.35,.75); glVertex3i (-lenth,0,lenth);
	glTexCoord2f (.35,1); glVertex3i (-lenth,lenth/2,lenth); 
	glEnd();	
		
	glBegin (GL_QUADS);
	glTexCoord2f (.35,0); glVertex3i (-lenth,lenth/2,lenth);
	glTexCoord2f (.35,.35); glVertex3i (-lenth,lenth/2,-lenth); 
	glTexCoord2f (.65,.35); glVertex3i (lenth,lenth/2,-lenth);
	glTexCoord2f (.65,0); glVertex3i (lenth,lenth/2,lenth); 
	glEnd();
	glPopAttrib();
}
void display()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glViewport (0,he - (wi/2), wi, wi/2);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluPerspective (60,2,.1,1000);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity(); 
	if (toggle == 1)
		gluLookAt (x-6*cos((angle)*3.14/180),6,z-6*sin((angle)*3.14/180), x+20*cos(angle*3.14/180), 5, z+20*sin(angle*3.14/180), 0,1,0);
	else
		gluLookAt (x-5*cos((angle-90)*3.14/180),5,z-5*sin((angle-90)*3.14/180), x,5,z, 0,1,0);

	glPushMatrix();	
	glTranslatef (0,-5,0);
	gridsky();
	glPopMatrix();

	x = current_origin_x + dis*cos(angle*3.14/180);
	z = current_origin_z + dis*sin(angle*3.14/180);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef (x,3,z);
	glRotatef (angle-90, 0,-1,0);
	glmDraw (plane, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
	glPopAttrib();
	//---------------------------------------------------------
	glViewport (wi-(he-(wi/2)),0,he-(wi/2),he-(wi/2));
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluPerspective (60,1,.1,1000);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();  
	//gluLookAt (x+cos(angle*3.14/180)*/, 10, z+sin(angle*3.14/180),  x+cos(angle*3.14/180), 0, z+sin(angle*3.14/180),  cos(angle*3.14/180),0,sin(angle*3.14/180));
	gluLookAt (x, 10, z,  x, 0, z,  cos(angle*3.14/180),0,sin(angle*3.14/180));
	glPushMatrix();	
	grid();	
	glPopMatrix();

	x = current_origin_x + dis*cos(angle*3.14/180);
	z = current_origin_z + dis*sin(angle*3.14/180);

	glTranslatef (x,0,z);
	glPushMatrix();
	glRotatef (angle, 0,-1,0);

	
	glPushMatrix();
	glBegin (GL_TRIANGLES);
		glVertex3f (0,0,0.5);
		glVertex3f (1.5,0,0);
		glVertex3f (0,0,-.5);
	glEnd();
	glPopMatrix();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3ub(255,0,0);
	
	glPushMatrix();
	BEGIN_2D_OVERLAY (15,15);
	glBegin( GL_QUADS );
		glVertex2f( 0, 1 );
		glVertex2f( 15, 1 );
		glVertex2f( 15, 0 );
		glVertex2f( 0, 0 );
	glEnd();
	glBegin( GL_QUADS );
		glVertex2f( 0, 14 );
		glVertex2f( 15, 14 );
		glVertex2f( 15, 15.0 );
		glVertex2f( 0, 15.0 );
	glEnd();
	glBegin( GL_QUADS );
		glVertex2f( 0, 15 );
		glVertex2f( 0, 0 );
		glVertex2f( 1, 0 );
		glVertex2f( 1, 15.0 );
	glEnd();
	glBegin( GL_QUADS );
		glVertex2f( 14, 0 );
		glVertex2f( 14, 15 );
		glVertex2f( 15, 15 );
		glVertex2f( 15.0, 0 );
	glEnd();
	END_2D_OVERLAY();
	glPopMatrix();
	//----------------------------------------------
	glViewport (0,0,wi-(he-(wi/2)),he-(wi/2));
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluPerspective (90,1,.1,1000);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity(); 

	gluLookAt (0, 20, 20,  0, 20, 0,  0,1,0);
	
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glBindTexture (GL_TEXTURE_2D, Dashboard);
	glEnable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);	
	glColor3ub (255,255,255);

	glBegin (GL_QUADS);
	glTexCoord2f (0,1); glVertex3i (-20,0,0);
	glTexCoord2f (0,0); glVertex3i (-20,40,0); 
	glTexCoord2f (1,0); glVertex3i (20,40,0); 
	glTexCoord2f (1,1); glVertex3i (20,0,0);
	glEnd();
	glPopAttrib();

	glColor3ub(255,0,255);
	BEGIN_2D_OVERLAY(15,15);
	drawstr (1,12, "time = %02d : %02d   X = %.2f   Z = %.2f  speed = %.f KM", count/60 ,count%60,x,z,10*speed);
	END_2D_OVERLAY();

	glutSwapBuffers();
}

void timer(int dummy)
{
	glutTimerFunc (100, timer, 0);
	static int last;
	int now;
	float dt;

	if (last == 0) {
		last = glutGet (GLUT_ELAPSED_TIME);
		return;
	}
	now = glutGet (GLUT_ELAPSED_TIME);
	dt = (now - last)/1000.;
	last = now;

	dis += speed*dt;
	glutPostRedisplay();
}
void timerclock (int dummy)
{
	glutTimerFunc (1000, timerclock, 0);
	count++;
}
void special (int key, int mx, int my)
{
	switch (key) {
	case GLUT_KEY_UP:
		if (speed < maxspeed)
			speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		if (speed > 0)
			speed -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		current_origin_x = x; current_origin_z = z;
		dis = 0.0;
		angle += 2;
		break;
	case GLUT_KEY_LEFT:
		current_origin_x = x; current_origin_z = z;
		dis = 0.0;
		angle -= 2;
		break;
	}
}
void keyboard (unsigned char key, int x, int y)//X,Y·Æ¹«¦ì¸m
{
	if (key == ' '){
		toggle  *= -1;	
	}

}
void init()
{
	glEnable (GL_DEPTH_TEST);

	glClearColor (.6,.6,.6,1);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	plane = glmReadOBJ("f-16.obj");
	glmUnitize(plane);
	glmFacetNormals(plane);
	glmVertexNormals(plane, 90.0);

	skybox = pngBind("skybox.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	
	Dashboard = pngBind("Dashboard.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
}
void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (800,600);
	glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	//viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	glutSpecialFunc (special);
	glutKeyboardFunc (keyboard);
	glutTimerFunc (0, timer, 0);
	glutTimerFunc (0, timerclock, 0);
	glutMainLoop();
}

