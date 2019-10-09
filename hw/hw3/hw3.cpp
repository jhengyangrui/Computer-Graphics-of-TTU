#include "gluit.h"
#include "svl/svl.h"
#include "gl/glpng.h"
//#include "GraphicsGems.h"
#include <math.h>

int mainwin, viewer;
GLMmodel *pmodel;
double cx=1,cy=1;
GLuint treetexture;
GLuint breakp;

void display(){}
void reshape (int w, int h)
{
//	glViewport(0,0,w,h);
	//ViewerReshape (viewer);
}

void grid_xy (void)
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	
	// grid line
	glColor3f (1,1,1);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex2i (i,-10);	glVertex2i (i,10);
		glVertex2i (-10,i);	glVertex2i (10,i);
	}
	glEnd();

	// axes
	glLineWidth (5.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0), glVertex3i (0,0,0); glVertex3i (1,0,0);
	glColor3f (0,1,0), glVertex3i (0,0,0); glVertex3i (0,1,0);
	glColor3f (0,0,1), glVertex3i (0,0,0); glVertex3i (0,0,1);
	glEnd();

	glPopAttrib();
}

float current_origin_x, current_origin_y;
float angle;
float _distance;  // somehow, "distance" collide with a system symbol

float speed = 1.0;
float maxspeed = 5.0;

// AABB for the arrow
Vec2 min(0,-.5), max(1.5,.5);
Vec2 points[5];

int isBound;

void check()
{ 
	int x = 5,y = 5,Rad = 1;
	isBound = 0;
	float Rmaxx = points[2][0]-x, Rmaxy = points[2][1]-y, Rminx = points[0][0]-x, Rminy = points[0][1]-y;
	if(Rmaxx < Rminx){//change 180
		float temp = Rmaxx;
		Rmaxx = Rminx;
		Rminx = temp;
		}
	if(Rmaxy < Rminy){
		float temp = Rmaxy;
		Rmaxy = Rminy;
		Rminy = temp;
	}
	if(Rmaxx < 0){
		if (Rmaxy < 0){
			if(Rmaxx*Rmaxx + Rmaxy*Rmaxy < Rad*Rad){
				speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
			}
		}
		else if (Rminy > 0){
			if(Rmaxx*Rmaxx + Rminy*Rminy < Rad*Rad){
				speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
			}
		}
		else if (-1*Rmaxx< Rad){
			speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
		}
	}
	else if (Rminx > 0){
		if (Rmaxy < 0){
			if(Rminx*Rminx + Rmaxy*Rmaxy < Rad*Rad){
				speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
			}
		}
		else if (Rminy > 0){
			if(Rminx*Rminx + Rminy*Rminy < Rad*Rad){
				speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
			}
		}
		else if (Rminx < Rad){
			speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
		}
	}
	else {
		if (Rmaxy < 0 && -1*Rmaxy < Rad){
			speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
		}
		else if (Rminy > 0 && Rminy < Rad){
			speed = 0.0;drawstr (1,3, "please back"); isBound = 1;
		}
	}
}

void DrawAABB ()
{
	// four points on the AABB
	points[0] = Vec2 (min[0],min[1]);
	points[1] = Vec2 (max[0],min[1]);
	points[2] = Vec2 (max[0],max[1]);
	points[3] = Vec2 (min[0],max[1]);
	points[4] = Vec2 (min[0]-2,min[1]);//...................



	// transform
	for (int i = 0; i < 5; i++) //...................
		//points[i] = proj(HTrans3 (Vec2 (current_origin_x, current_origin_y)) * HRot3 (angle*vl_pi/180) * HTrans3 (Vec2(_distance,0)) * Vec3(points[i],1.0)); 
    check();
	// display
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glColor3f (1,0,1);
	glBegin (GL_LINE_LOOP);
	for (i = 0; i < 4; i++) //...................
		glVertex2dv (points[i].Ref());
	glEnd();
	glPopAttrib();	
	//...................
}

void DrawVehicle ()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glColor3f (1,1,0);
	// bounding box: [0,-.5] x [1.5,.5]
	glBegin (GL_TRIANGLES);
		glVertex2f (0,0.5);
		glVertex2f (1.5,0);
		glVertex2f (0,-.5);
	glEnd();
	glPopAttrib();
}


void drawXtree (const Vec3& center, const Vec3& size)
{
    glPushAttrib (GL_ALL_ATTRIB_BITS);

    
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, treetexture);
	glEnable (GL_ALPHA_TEST);
	glDisable (GL_CULL_FACE);
	glAlphaFunc (GL_GREATER, 0);
	glPushMatrix();
	    glRotatef (90., 1,0,0);
	    glTranslated (center[0], center[1], center[2]);
		glScaled (size[0], size[1], size[2]);
		glBegin (GL_QUADS);
	    glTexCoord2f (0,0), glVertex2i (-1, 0);
	    glTexCoord2f (1,0), glVertex2i (1, 0);
	    glTexCoord2f (1,1), glVertex2i (1, 2);
	    glTexCoord2f (0,1), glVertex2i (-1, 2);
	    glEnd();
 
		glRotatef (90., 0.,1.,0.);
		glBegin (GL_QUADS);
	    glTexCoord2f (0,0), glVertex2i (-1, 0);
	    glTexCoord2f (1,0), glVertex2i (1, 0);
	    glTexCoord2f (1,1), glVertex2i (1, 2);
	    glTexCoord2f (0,1), glVertex2i (-1, 2);
	    glEnd();
    glPopMatrix();

	glPopAttrib ();
}

void content (void)
{
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(	100, 1,	5, 20);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	
	if(isBound == 1)
		gluLookAt(points[0][0],0,points[0][1]*(-1), points[0][0]*2-points[4][0],-5,points[0][1]*(-1)*2-points[4][1]*(-1), 0,1,0);
	else
		gluLookAt(points[4][0],5,points[4][1]*(-1), points[0][0],0,points[0][1]*(-1), 0,1,0);//...................
    
	glRotatef (-90, 1,0,0);
	grid_xy();

	glPushMatrix();
	glTranslatef (current_origin_x, current_origin_y, 0);
	glRotatef (angle, 0, 0,1);    
	glTranslatef (_distance, 0,0);
	
	glPushMatrix();
	glRotatef (90, 1,0,0);
	glRotatef (90, 0,1,0);

	glmDraw (pmodel, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
	
	glPopMatrix();
	
	// draw AABB in world coordinates
	DrawAABB();

	glColor3ub (255,0,255);
	BEGIN_2D_OVERLAY(15,15);
	drawstr (1,2, "up/down to accel/decel");
	drawstr (1,1, "left/right to turn");
	END_2D_OVERLAY();

	glPushMatrix();
	glTranslatef (5, 5, 0);
	drawXtree (Vec3 (0,0,0), Vec3 (1,2,.5));
	glPopMatrix();

	
	
	glutSwapBuffers();
}

void idle(void)
{
	static int last;
	int now;
	float dt;

	if (last == 0) {
		last = glutGet (GLUT_ELAPSED_TIME);
		return;
	}
	now = glutGet (GLUT_ELAPSED_TIME);
	dt = (now - last)/1000.;  // in "seconds"
	last = now;

	_distance += speed*dt;
	glutPostRedisplay();
	//ViewerRedisplay(viewer);
}

void special (int key, int mx, int my)
{
	switch (key) {
	case GLUT_KEY_UP:
		if (speed < maxspeed)
			speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		//if (speed > 0)
			speed -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		current_origin_x += _distance*cos(angle*3.14/180); 
		current_origin_y += _distance*sin(angle*3.14/180);
		_distance = 0.0;
		angle -= 2;
		break;
	case GLUT_KEY_LEFT:
		current_origin_x += _distance*cos(angle*3.14/180); 
		current_origin_y += _distance*sin(angle*3.14/180);
		_distance = 0.0;
		angle += 2;
		break;
	}
}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	pmodel = glmReadOBJ("porsche.obj");
	glmUnitize(pmodel);
	glmFacetNormals(pmodel);
	glmVertexNormals(pmodel, 90.0);

	pngSetStandardOrientation(1); // important to set "before" loading
	// remember set to PNG_ALPHA to read in the alpha channel
	// set to GL_LINEAR (better than GL_NEAREST)
	treetexture = pngBind("shrub1.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
    breakp = pngBind("break.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	glClearColor (.4,.4,.4,1);

}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE |GLUT_DEPTH);
	glutInitWindowSize (400,400);

	mainwin = glutCreateWindow ("drive");
	glutDisplayFunc (content);
	glutReshapeFunc (reshape);

	//viewer = ViewerNew (mainwin, 0,0,400,400, content);
	//ViewerExtent (viewer, 20);	

	init();
	glutSpecialFunc (special);
	glutIdleFunc (idle);

	glutMainLoop();
}


