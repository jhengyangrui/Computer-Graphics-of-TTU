#include <stdlib.h>
#include <gl/glut.h>

#include "ccd_box.h"

static Vec2 myq(0,0); // keep the current joint angle
double l1 = 5.0;
double l2 = 5.0;
Vec3 takepoint;
#define drawOneLine(x1,y1,x2,y2) \
{glBegin (GL_LINES); glVertex3f(x1,0,y1);glVertex3f(x2,0,y2); glEnd();}

vector <CCD_axis> axes;

void setarm()
{
	CCD_axis link;
	link.axis = Vec3 (0,1,0); link.jointid = 0; link.limits = Vec2(-1e10,1e10); axes.push_back(link);
	link.axis = Vec3 (0,1,0);	link.jointid = 1; link.limits = Vec2(-1e10,1e10); axes.push_back(link);	
	//link.axis = Vec3 (0,0,1);	link.jointid = 1; link.limits = Vec2(-0.1, 1.3); axes.push_back(link);	
}

void drawarm()
{
	glLineWidth (3.0);
	glPushMatrix();	
	glRotatef (myq[0]*180/vl_pi, 0,1,0);
	glColor3ub (255,0,0);
	drawOneLine (0,0,l1,0);
	glTranslatef (l1,0,0);
	glRotatef (myq[1]*180/vl_pi, 0,1,0);
	glColor3ub (0,255,255);
	drawOneLine (0,0,l2,0);

	glColor3ub (255,0,0);
	glPointSize (20.0);
	glBegin (GL_POINTS);
	    glVertex3f (l1,0,0);
	glEnd();

	glPopMatrix();
}
 
void fk (const Vec& q, vector<Vec3>& joints)  // joints[0] as base
{
	// assume base is origin
	joints[0] = Vec3(0,0,0);
	joints[1] = proj (HRot4 (Vec3(0,1,0),q[0]) * HTrans4 (Vec3(l1,0,0)) * Vec4 (0,0,0,1)); 
	joints[2] = proj (HRot4 (Vec3(0,1,0),q[0]) * HTrans4 (Vec3(l1,0,0)) * HRot4 (Vec3(0,1,0),q[1]) * HTrans4 (Vec3(l2,0,0)) * Vec4 (0,0,0,1)); 
	takepoint = joints[2];
}

int myik (const Vec3& mytarget)
{
	Vec q (2, vl_0);
	
	int i;
	for (i = 0; i < q.Elts(); i++)
		q[i] = myq[i];
	
	extern int ik_ccd (const Vec3&, Vec&);
	
	int reachable = ik_ccd (mytarget, q);
	for (i = 0; i < 2; i++)
		myq[i] = q[i];
	return reachable;
}
