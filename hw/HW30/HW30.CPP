#include <stdlib.h>
#include "gluit.h"
#include <gl/glut.h>
#include "gl/glpng.h"
#include <md2m.h>
#include "svl/svl.h"
//#include <math.h>

//#pragma comment (lib, "svl-dbg.lib")

int toggle = 1;
// global variables
t3DModel *AAA;
float scale, center[3], size[3];
int loopover;

GLuint wall;
void grid()
{
	glColor3f (1,1,1);
	glLineWidth (1);
	glBegin (GL_LINES);
	for (int i = -5; i <= 5; i++) {
		glVertex3i (-5,0,i); glVertex3i (5,0,i);
		glVertex3i (i,0,-5); glVertex3i (i,0,5);		
	}
	glEnd();
}

void circle()
{
	glColor3f (1,0,0);
	glLineWidth (5);
	glBegin (GL_LINE_LOOP);
	double dq;
	dq = 2*3.14/30;
	for (int i =0; i < 30; i++) {
		glVertex3d (2*cos(i*dq), 0, 2*sin(i*dq));
	}
	glEnd();
}

Vec3 point[3];

void init()
{
	point[0] = Vec3 (0,0,0.5);
	point[1] = Vec3 (1.5,0,0);
	point[2] = Vec3 (0,0,-0.5);
	// init
	AAA = MD2MReadMD2 ("astro.md2", "astro.png");
	MD2MFindDimension  (AAA, center, size);
	#define max(x,y) ((x)>(y)?(x):(y))
	#define max3(x,y,z) (max(max(x,y),max(y,z)))
	scale = max3 (size[0],size[1],size[2]);
	#undef max
	#undef max3
	MD2MSetLoop (AAA, GL_TRUE); 
	MD2MSetAnimation (AAA, 1);

	pngSetStandardOrientation (1);

	//glEnable (GL_TEXTURE_2D);
	wall = pngBind("123.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	//MD2MSetAnimationSpeed (AAA, float Hz); 

}

void draw()
{
	glBegin(GL_TRIANGLES);
		glVertex3dv (point[0].Ref());
		glVertex3dv (point[1].Ref());
		glVertex3dv (point[2].Ref());
	glEnd(); 
}

float angle = 30;
void display()
{
	// pipeline
	glViewport (0,0,400,400);
	
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); 
	//gluOrtho2D (-3,3,-3,3);
	gluPerspective (80, 1, 1, 100);
	
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	// svl tranformation
	Vec3 tpoint[3];
	int i;
	for (i = 0; i < 3; i++) {
		tpoint[i] = proj (HRot4 (Vec3(0,1,0), (angle+90)/180*vl_pi) * HTrans4(Vec3(0,0,2)) * Vec4(point[i],1));
	}
	// camera
	if (toggle == 1)
		gluLookAt (7,7,7, 0,0,0, 0,1,0);
	else
		gluLookAt ((tpoint[0][0]+tpoint[2][0])/2,(tpoint[0][1]+tpoint[2][1])/2+3,(tpoint[0][2]+tpoint[2][2])/2, tpoint[1][0],tpoint[1][1],tpoint[1][2], 0,1,0);

	// MV = I
	
	glClear (GL_COLOR_BUFFER_BIT);
	
	grid();
	circle();
	
	BEGIN_2D_OVERLAY (40,40);
		drawstr(3,38,"c for change" );
	END_2D_OVERLAY();
	

	glColor3f (0,1,1);


	
#if 0	
	glPushMatrix();
	glBegin (GL_TRIANGLES);
	for (i = 0; i < 3; i++) glVertex3dv (tpoint[i].Ref());
	glEnd();
	glPopMatrix();	

	glPushMatrix();  // stack: I, I
	glRotatef (angle+90, 0,1,0);
	glTranslatef (0, 0, 2);
	draw();
	glPopMatrix();
#endif
	glColor3f (1,1,1);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	/*glDisable (GL_LIGHTING); 
	glEnable (GL_DEPTH_TEST);*/
	glEnable (GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef (tpoint[0][0],tpoint[0][1],tpoint[0][2]);
	glRotatef (angle+90, 0,1,0);
	//glTranslatef (0,1,0);
	glScalef (2/scale, 2/scale, 2/scale);
	//glTranslatef (-center[0],-center[1],-center[2]);
	loopover = MD2MInterpolate (AAA);
	glPopMatrix();
	glPopAttrib();
	
	//glEnable (GL_TEXTURE_2D);  // turn on 2D texturing
	glDisable (GL_LIGHTING);   // no lighting (no normals)
	glEnable (GL_CULL_FACE);   // if cull-face desired
	glCullFace (GL_FRONT);     // cull the CCW (front) faces 
	glutSwapBuffers();
}

int toggle_motion = 1;

void timer (int dummy)
{
	if (toggle_motion) {
		angle += 6;
		glutPostRedisplay();
	}
	glutTimerFunc (100, timer, 0);
	
}

void mouse (int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		toggle_motion ^= 1;  // XOR 
	}
}
void keyboard (unsigned char key, int x, int y)
{
	if (key == 't') {
		toggle_motion ^= 1;  // XOR 
	}
	
	if (key == 'c'){
		toggle  *= -1;	
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitWindowSize (400,400);
	glutInitWindowPosition (200,200);

	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("test");
	
	init();
	
	//glClearColor (1,1,1,1);

	glutDisplayFunc (display);
	//glutIdleFunc (idle);
	glutTimerFunc (0, timer, 0);
	glutMouseFunc (mouse);
	glutKeyboardFunc (keyboard);
	
	glutMainLoop();
}