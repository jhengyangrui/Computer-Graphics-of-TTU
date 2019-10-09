#include "gluit.h"
#include "gl/glpng.h"
#include "svl/svl.h"

#pragma comment (lib, "glpngd-vc9.lib")
#pragma comment (lib, "gluit-vc9.lib")

int mainwin, viewer;

pngRawInfo info;

#define MAX 100.
#define MIN -100.
#define GRIDPTS   30       // THE SIZE OF PNG
#define INTERVALS (GRIDPTS-1)
float y[GRIDPTS][GRIDPTS];


float x, z;    // current pos
float current_origin_x, current_origin_z;

float angle;
float dis;
float speed = 1.0;
float maxspeed = 5.0;
GLMmodel *plane;

int wi,he;

void reshape (int w, int h)
{	
	wi = w;
	he = h;
}
float bilinear (float x, float z)
{
	float spacing = (MAX-MIN)/INTERVALS;
	int i = (x-MIN)/spacing;
	int j = (z-MIN)/spacing;
	
	float s = (x- (MIN+spacing*i))/spacing;
	float t = (z- (MIN+spacing*j))/spacing;

	return (1-t)*(1-s)*y[i][j] + (1-t)*s*y[i+1][j] + t*(1-s)*y[i][j+1] + s*t*y[i+1][j+1];
}

void setheight (void)
{
	for (int i = 0; i < GRIDPTS; i++) 
		for (int j = 0; j < GRIDPTS; j++) {
			y[j][i] = 20.0*(255-info.Data[i*info.Width+j])/255;  // [0.0, 20.0]
		}
}

Vec3 move = Vec3(0,0,0);
void init (void)
{
//	pngSetStandardOrientation (1);  // do not use
	glEnable (GL_DEPTH_TEST);
	glClearColor (.6,.6,.6,1);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	// grey scale image: 
	// [format: luminance] [type: unsigned_byte]
	// already taken care of by glpng lib

	pngLoadRaw ("terrain3.png", &info);
	setheight ();
	move[1] = bilinear (move[0], move[2]);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	plane = glmReadOBJ("toycar.obj");
	glmUnitize(plane);
	glmFacetNormals(plane);
	glmVertexNormals(plane, 90.0);
	glmScale(plane,10);
}

GLuint texid;  

void inittex()
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
//	pngSetStandardOrientation (1);  // do not use
	
	// grey scale image: 
	// [format: luminance] [type: unsigned_byte]
	// already taken care of by glpng lib
	texid = pngBind("terrain3.png", PNG_NOMIPMAP, PNG_SOLID, NULL, 
					GL_CLAMP, GL_LINEAR, GL_LINEAR);

	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	float splanes[] = {1./(MAX-MIN), 0,0,-MIN/(MAX-MIN)};
	float tplanes[] = {0,0,1./(MAX-MIN),-MIN/(MAX-MIN)};
	
	// so that (s,t) is in [0,1]x[0,1]
	glTexGenfv (GL_S, GL_OBJECT_PLANE, splanes);
	glTexGenfv (GL_T, GL_OBJECT_PLANE, tplanes);
}

GLuint pmode = GL_LINE;
void terrain (void)
{
    float spacing = (MAX-MIN)/INTERVALS;

	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glPolygonMode (GL_FRONT_AND_BACK, pmode);
	
	// terrain dimension
	// [MIN, MAX] x [0, 20] x [MIN, MAX]
	glBegin (GL_QUADS);
	for (int i = 0; i < INTERVALS; i++) {
		for (int j = 0; j < INTERVALS; j++) {
			glVertex3f (MIN+i*spacing, y[i][j], MIN+j*spacing);
			glVertex3f (MIN+(i+1)*spacing, y[i+1][j], MIN+j*spacing);
			glVertex3f (MIN+(i+1)*spacing, y[i+1][j+1], MIN+(j+1)*spacing);
			glVertex3f (MIN+i*spacing, y[i][j+1], MIN+(j+1)*spacing);
		}
	}
	glEnd();
	glPopAttrib();
}

void display (void) {

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glViewport (0,0, wi, he-he*(1/3));
	//glViewport (0,0, 800, 400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluPerspective (60,2,.1,1000);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity(); 
		
	gluLookAt (00,50,80 ,0,0,-50, 0,1,0);
	
	glDisable(GL_LIGHTING);
	glColor3f (1,1,1);
	terrain();
	
	move[0] = current_origin_x + dis*cos(angle*3.14/180);
	move[2] = current_origin_z + dis*sin(angle*3.14/180);

	move[1] = bilinear (move[0], move[2]);

	Vec3 p0 = Vec3(move[0] - cos((angle-90)*3.14/180) * 20, move[1], move[2] - sin((angle-90)*3.14/180) * 20);
	Vec3 p1 = Vec3(move[0] + cos((angle-90)*3.14/180) * 20, move[1], move[2] + sin((angle-90)*3.14/180) * 20);
	Vec3 p2 = Vec3(move[0] + cos(angle*3.14/180) * 20, move[1], move[2] + sin(angle*3.14/180) * 20);
	Vec3 pxz = p2 - move;
	//Vec3 pxz = Vec3(move[0] + cos(angle*3.14/180) * 20, 0, move[2] + sin(angle*3.14/180) * 20);


	p0[1] = bilinear (p0[0],p0[2]);
	p1[1] = bilinear (p1[0],p1[2]);
	p2[1] = bilinear (p2[0],p2[2]);
	//pxz[1] = bilinear (pxz[0],pxz[2]);

	Vec3 yy = norm(cross((p1-p0),(p2-p0)));
	Vec3 xx = norm(pxz - dot(yy,pxz)*yy);
	Vec3 zz = cross(xx,yy);
	
	double mat[16] = { xx[0], xx[1], xx[2], 0.0, 
		               yy[0], yy[1], yy[2], 0.0, 
					   zz[0], zz[1], zz[2] , 0.0,
					   move[0], move[1]+5, move[2], 1.0};

	/*glColor3f (1,0,0);
	glBegin(GL_TRIANGLES);
	glVertex3f (p0[0], p0[1]+3, p0[2]);
	glVertex3f (p1[0], p1[1]+3, p1[2]);
	glVertex3f (p2[0], p2[1]+3, p2[2]);
	glEnd();*/

	//glTranslatef (move[0],move[1]+5,move[2]);
	//glRotatef (180, 1,0,0);
	//glRotatef (-90, 0,-1,0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glMultMatrixd(mat);
	glRotatef (90, 0,-1,0);
	glRotatef (180, 1,0,0);
	glmDraw (plane, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
	glPopAttrib();
//------------------------------------------------------------------------------------------------
	
	//glViewport ((3/4)*wi,(2/3)*he, 100, 100);
	glViewport (wi - he/3 ,he - he/3 ,he/3-10 ,he/3-10);

	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluPerspective (60,2,.1,1000);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity(); 
	gluLookAt (0,100,0 ,0,0,0, 0,0,-1);
	
	glDisable(GL_LIGHTING);
	glColor3f (1,1,1);
	terrain();

	glPushAttrib (GL_ENABLE_BIT);
	glColor3f (1,0,0);
	glTranslatef (0,5,0);
	glPointSize (5);
	glDisable (GL_TEXTURE_2D);
	glEnable (GL_POINT_SMOOTH);
	glBegin (GL_POINTS);
	glVertex3dv (move.Ref());
	glEnd();
	glPopAttrib();
	glutSwapBuffers();
}


void content (void) 
{
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
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'l': case 'L':
		pmode = GL_LINE;
		break;
	case 't': case 'T':
		glEnable (GL_TEXTURE_2D);
		pmode = GL_FILL;
		break;
	}
	//ViewerRedisplay(viewer);
	glutPostRedisplay();
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
		current_origin_x = move[0]; current_origin_z = move[2];
		dis = 0.0;
		angle += 2;
		break;
	case GLUT_KEY_LEFT:
		current_origin_x = move[0]; current_origin_z = move[2];
		dis = 0.0;
		angle -= 2;
		break;
	}
}

void main (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (800,600);
	glutCreateWindow ("grey [L|T]");

	//mainwin = glutCreateWindow ("grey [L|T]");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	//viewer = ViewerNew (mainwin, 0,0,400,400, content);
	//ViewerExtent (viewer, 200);
	glutKeyboardFunc (keyboard);
	init();
	inittex ();
	glutSpecialFunc (special);

	extern void special(int,int,int);
	glutTimerFunc (0, timer, 0);

	glutMainLoop();
}
