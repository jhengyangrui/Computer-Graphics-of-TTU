#include <stdio.h>
#include "gluit.h"
#include <gl/glpng.h>


void timer (int dummy);
int mainwin, viewer;
GLuint id1,id2,id3,id4,id5,id6,id7,id8;
float movelenth = 0, movelenth2 = 0;
float theta;

float point0[3] = {-3.5, 7, 3.5};
float point1[3] = {-3.5, 0, 3.5};
float point2[3] = {3.5, 0, 3.5};
float point3[3] = {3.5, 7, 3.5};
float point4[3] = {3.5, 0, -3.5};
float point5[3] = {3.5, 7, -3.5};
float point6[3] = {-3.5, 7, -3.5};
float point7[3] = {-3.5, 0, -3.5};
float point8[3] = {-3.5, 10, 0};
float point9[3] = {0, 10, 0};
float point10[3] = {-3.5, 11, 0};
float point11[3] = {0, 11, 0};
float point12[3] = {-3.5, 11, -0.1};
float point13[3] = {0, 11, -0.1};
float point14[3] = {0, 10, -0.1};
float point15[3] = {-3.5, 10, -0.1};
float point16[3] = {3.5, 10, 0};
float point17[3] = {3.5, 11, 0};
float point18[3] = {3.5, 10, -0.1};
float point19[3] = {3.5, 11, -0.1};

void grid()
{
	glColor3f (1,1,1);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10,0,i); glVertex3i (10,0,i);
		glVertex3i (i,0,-10); glVertex3i (i,0,10);
	}
	glEnd();
}

void model ()
{
	glBindTexture(GL_TEXTURE_2D, id2); 
	glBegin(GL_QUADS);	
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 1); glVertex3fv(point0);
	glTexCoord2f(0, 0); glVertex3fv(point1);
	glTexCoord2f(1, 0); glVertex3fv(point2);
	glTexCoord2f(1, 1); glVertex3fv(point3);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id3); 
	glBegin(GL_QUADS);	
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 1);glVertex3fv(point3);
	glTexCoord2f(0, 0);glVertex3fv(point2);
	glTexCoord2f(1, 0);glVertex3fv(point4);
	glTexCoord2f(1, 1);glVertex3fv(point5);
	glEnd();

    glBindTexture(GL_TEXTURE_2D, id4); 
    glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 1);glVertex3fv(point5);
	glTexCoord2f(0, 0);glVertex3fv(point4);
	glTexCoord2f(1, 0);glVertex3fv(point7);
	glTexCoord2f(1, 1);glVertex3fv(point6);
	glEnd();

    glBindTexture(GL_TEXTURE_2D, id1); 
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1);glVertex3fv(point6);
	glTexCoord2f(0, 0);glVertex3fv(point7);
	glTexCoord2f(1, 0);glVertex3fv(point1);
	glTexCoord2f(1, 1);glVertex3fv(point0);
	glEnd();
	
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);glVertex3fv(point7);glVertex3fv(point4);glVertex3fv(point2);glVertex3fv(point1);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, id5); 
    glBegin(GL_QUADS);
	glNormal3f(0, 3/4.6, 3.5/4.6);
	glTexCoord2f(0, 1);glVertex3fv(point8);
	glTexCoord2f(0, 0);glVertex3fv(point0);
	glTexCoord2f(1, 0);glVertex3fv(point3);
	glTexCoord2f(0.5, 1);glVertex3fv(point9);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id6); 
    glBegin(GL_QUADS);
	glNormal3f(0, 3/4.6, -3.5/4.6);
    glTexCoord2f(1, 1);	glVertex3fv(point8);
	glTexCoord2f(0.5, 1); glVertex3fv(point9);
	glTexCoord2f(0, 0); glVertex3fv(point5);
	glTexCoord2f(1, 0); glVertex3fv(point6);
	glEnd();

	glBegin(GL_QUADS);//程オ娩
	glNormal3f(0, 0, 1);glVertex3fv(point10);glVertex3fv(point8);glVertex3fv(point9);glVertex3fv(point11);
	glNormal3f(0, 0, -1);glVertex3fv(point12);glVertex3fv(point13);glVertex3fv(point14);glVertex3fv(point15);
	glNormal3f(0, 1, 0);glVertex3fv(point12);glVertex3fv(point13);glVertex3fv(point10);glVertex3fv(point11);
	glEnd();

	glBegin(GL_QUADS);//程娩
	glNormal3f(0, 0, 1);glVertex3fv(point11);glVertex3fv(point9);glVertex3f(point16[0]-movelenth,point16[1]-0.3*movelenth,point16[2]+movelenth);glVertex3f(point17[0]-movelenth,point17[1]-0.3*movelenth,point17[2]+movelenth);
	glNormal3f(0, 0, -1);;glVertex3f(point19[0]-movelenth,point19[1],point19[2]-movelenth);glVertex3f(point18[0]-movelenth,point18[1],point18[2]-movelenth);glVertex3fv(point14);glVertex3fv(point13);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id5); 
    glBegin(GL_TRIANGLES);//タà
	glNormal3f(0, 1, 1);
    glTexCoord2f(0.5, 1);	glVertex3fv(point9);
	glTexCoord2f(1, 0); glVertex3fv(point3);
	glTexCoord2f(1, 1); glVertex3f(point16[0]-movelenth,point16[1]-0.3*movelenth,point16[2]+movelenth);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id6); 
    glBegin(GL_TRIANGLES);//璉à
	glNormal3f(0, 3/4.6, -3.5/4.6);
    glTexCoord2f(0, 1);	;glVertex3f(point18[0]-movelenth,point18[1],point18[2]-movelenth);
	glTexCoord2f(0, 0); glVertex3fv(point5);
	glTexCoord2f(0.5, 1); glVertex3fv(point9);
	glEnd();

    glBindTexture(GL_TEXTURE_2D, id7); 
    glBegin(GL_TRIANGLES);//タず场à
	glNormal3f(1, 1, -1);
    glTexCoord2f(0.5, 1);glVertex3f(point9[0]+movelenth2,point9[1],point9[2]);
	glTexCoord2f(0, 1); glVertex3f(point16[0]-movelenth,point16[1]-0.3*movelenth,point16[2]+movelenth);
	glTexCoord2f(0, 0); glVertex3fv(point3);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id7); 
    glBegin(GL_QUADS);//タず场àよ
	glNormal3f(1, 1, -1);
    glTexCoord2f(0, 1);glVertex3f(point17[0]-movelenth,point17[1]-0.3*movelenth,point17[2]+movelenth);
	glTexCoord2f(0, 0.9);glVertex3f(point16[0]-movelenth,point16[1]-0.3*movelenth,point16[2]+movelenth);
	glTexCoord2f(0.5, 0.9);glVertex3fv(point9);
	glTexCoord2f(0.5, 1);glVertex3fv(point11);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id7); 
    glBegin(GL_TRIANGLES);//タず场タà
	glNormal3f(1, 0, 0);
    glTexCoord2f(0.5, 1);glVertex3f(point9[0]+movelenth2,point9[1],point9[2]);
	glTexCoord2f(0, 0); glVertex3fv(point3);
	glTexCoord2f(1, 0); glVertex3fv(point5);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id7); 
    glBegin(GL_TRIANGLES);//璉ず场à
	glNormal3f(1, 1, -1);
    glTexCoord2f(0.5, 1);glVertex3f(point9[0]+movelenth2,point9[1],point9[2]);
	glTexCoord2f(1, 0); glVertex3fv(point5);
	glTexCoord2f(1, 1); glVertex3f(point18[0]-movelenth,point18[1]-0.3*movelenth,point18[2]-movelenth);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, id7); 
    glBegin(GL_QUADS);//璉ず场àよ
	glNormal3f(1, 1, -1);
    glTexCoord2f(1, 0.9);glVertex3f(point18[0]-movelenth,point18[1]-0.3*movelenth,point18[2]-movelenth);
	glTexCoord2f(1, 1);glVertex3f(point19[0]-movelenth,point19[1]-0.3*movelenth,point19[2]-movelenth);
	glTexCoord2f(0.5, 1);glVertex3fv(point11);
	glTexCoord2f(0.5, 0.9);glVertex3fv(point9);
	glEnd();

	glBegin(GL_QUADS);//ず┏场
	glNormal3f(0, 1, 0);glVertex3fv(point6);glVertex3fv(point0);glVertex3fv(point3);glVertex3fv(point5);
	glEnd();

    glBindTexture(GL_TEXTURE_2D, id8); 
    glBegin(GL_TRIANGLES);//璉ず场à
	glNormal3f(-1, 0, 0);
    glTexCoord2f(0.5, 1);glVertex3fv(point8);
	glTexCoord2f(0, 0); glVertex3fv(point6);
	glTexCoord2f(1, 0); glVertex3fv(point0);
	glEnd();

}


void display()
{
	// viewport animation

	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	grid();
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 90);        // default 180
	glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, 2); 
	
	// rotating spotlight
	float pos[4] = {0,0,0,1};
	float dir[3] = {0,0,-1};

	glDisable (GL_LIGHTING);
	glColor3f (1,1,0);
	glPushMatrix();
	glRotatef (theta, 0,1,0);
	glTranslatef (0,10,10);
	
	glutWireCone (1,1,10,10);
	
	glLightfv (GL_LIGHT0, GL_POSITION, pos);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	
	glPopMatrix();
	glEnable (GL_LIGHTING);

	model();
	
	glutSwapBuffers();
}
void timer2 (int dummy)
{
	static int sign = 1;
	theta += 5*sign;

	if (movelenth2  > 0 )
	    movelenth2 = movelenth2-0.5;
	if (movelenth2  <= 0 && movelenth >= 0 ){		
			movelenth = movelenth-0.5;
	}
	if (movelenth  <=0 && movelenth2 <=0){
		glutTimerFunc (100, timer, 0);
		return;
	}
	


	glutTimerFunc (100, timer2, 0);
	glutPostRedisplay();
}

void timer (int dummy)
{
	static int sign = 1;
	theta += 5*sign;

	if (movelenth  < 3 )
	    movelenth = movelenth+0.5;
	if (movelenth  >= 3 && movelenth2 < 5 ){		
			movelenth2 = movelenth2+0.5;
	}
	if (movelenth  >= 3 && movelenth2 >= 5){
		glutTimerFunc (100, timer2, 0);
		return;
	}
	


	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}
void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glClearColor (.4,.4,.4,1);

	pngInfo info; 
	glEnable (GL_TEXTURE_2D);pngSetStandardOrientation(1);
	 id1 = pngBind("1-1.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id2 = pngBind("1-2.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);  	 
     id3 = pngBind("1-3.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id4 = pngBind("1-4.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id5 = pngBind("1-5.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id6 = pngBind("1-6.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id7 = pngBind("1-7.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 id8 = pngBind("1-8.PNG", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST); 
 	 ViewerCenter(viewer,3.5,3.5,7);
	 ViewerExtent(viewer,20);

}

void main (int argc, char** argv)
{	
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, display);
	init();
	glutTimerFunc (100, timer, 0);

	glutMainLoop();

}
