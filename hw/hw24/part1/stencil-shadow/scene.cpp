#include <stdio.h>
#include <gl/glew.h>
#include "gl/glpng.h"
#include "gluit.h"
#include "svl/svl.h"
#include "md2m.h"
#include "teapot.h"
#include <math.h>


static float lightPosition[4] = {-1.0, 1.0, 3.0, 1.0};

//GLMmodel *pmodel;

Teapot teapots[3];
int nteapots = 3;
float scale, center[3], size[3];
t3DModel *yoshi;
int pose, oldpose;   // 0 (stand), 1 (run), 6 (jump) 
Vec3 point;
int loopover;
float speed;

#define max2(x,y) ((x)>(y)?(x):(y))
#define max3(x,y,z) (max2(max2(x,y),max2(y,z)))

void init_md2()
{
	// load MD2 model
	yoshi = MD2MReadMD2 ("yoshi-tris.md2", "yoshi.png");
	MD2MFindDimension  (yoshi, center, size);
	scale = max3 (size[0],size[1],size[2]);

	MD2MSetLoop (yoshi, GL_TRUE); 
	MD2MSetAnimation (yoshi, pose);  // 0: stand
	MD2MSetAnimationSpeed (yoshi, 10); 
}
void initOccluder ()
{
	/*pmodel = glmReadOBJ ("al.obj");
	glmUnitize (pmodel);
	glmFacetNormals (pmodel);
	glmVertexNormals (pmodel, 90.0);*/
	double h = 2.0; // height of teapot
	teapots[0].pos = Vec3 (2,h,3); teapots[0].score = 10;
	teapots[1].pos = Vec3 (0,h,0); teapots[1].score = 10;
	teapots[2].pos = Vec3 (2,h,-3); teapots[2].score = 10;
	init_md2();
}



static GLuint FloorTexture;

void initFloor(char* filename)
{
	pngSetStandardOrientation (1);
	FloorTexture = pngBind(filename, PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}
//////////////////////////////////
// grab teapot
int total_score;
int hit = -1;

// Grab: return [0, n-1] for successful grabbing, 
//       return -1 if nothing grabbed
int Grab (Vec3 point)  // point: (x,0,z)
{
	double eps = 1.5;  // threshold for successful grabbing
	
	for(int i = 0; i < 3; i++) {
		if(teapots[i].status == 1){
				if(sqrt((teapots[i].pos[0]-point[0])*(teapots[i].pos[0]-point[0])+(teapots[i].pos[2]-point[2])*(teapots[i].pos[2]-point[2])) <= eps){
					return i;
				}
		}
				else
					return -1;
	}
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void pose_fsm ()
{
	switch (pose) {
	case 0: // stand
		if (speed > 0) {
			pose = 1;   // stand --> run
			MD2MSetAnimation (yoshi, pose);
		}
		break;
	case 1: // run
		if (speed == 0.0) {
			pose = 0;   // run --> stand
			MD2MSetAnimation (yoshi, pose);
		}
		break;
	case 6: // jump
		if (loopover == 1) {
			pose = oldpose;   // when jump is done --> oldpose (stand or run)
			MD2MSetLoop (yoshi, GL_TRUE);
			MD2MSetAnimation (yoshi, pose);
		}
		break;
	}
}
float angle;

void DrawOccluder()
{
	/*glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_TEXTURE_2D);
	glPushMatrix();
		glRotatef (angle, 0,1,0);
		glTranslatef (2,0,0);
		glRotatef (180, 0,1,0);
		glTranslatef(0,1,0);
		glmDraw (pmodel, GLM_MATERIAL|GLM_SMOOTH); 
	glPopMatrix();
	glPopAttrib();*/
	//glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glTranslated (point[0], point[1],point[2]);
	glRotatef (angle, 0,1,0);

	//glTranslatef (0,1,0);
	glScalef (2/scale, 2/scale, 2/scale);
	glTranslatef (-center[0],-center[1],-center[2]);
	loopover = MD2MInterpolate (yoshi);
	glPopMatrix();


	// teapots
	for (int i = 0; i < nteapots; i++) {
		teapots[i].show();
	}
	/*glPushMatrix();
	glDisable (GL_TEXTURE_2D);
	glTranslatef (2,2,3);
	glutSolidTeapot (1.0);
	glPopMatrix();*/

	BEGIN_2D_OVERLAY (10,10);
	glDisable (GL_TEXTURE_2D);
	glColor3f (1,1,0);
	char display[20];
	sprintf (display, "Score: %d", total_score);
	drawstr (1,1, display);
	END_2D_OVERLAY();

	//glutSwapBuffers();
    // glUniform1i (texedobjloc, 1);  // for textured objects
    // glUniform1i (texedobjloc, 0); for un-textured objects: 	
}

void DrawFloor(float fCenterX, float fCenterY, float fCenterZ)
{
	float FloorColor[] = {1,1,1,0.6};
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, FloorColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);


	// set up blending so we can see the reflected cube through the
	// surface, and thus create the illusion of reflection

	glBindTexture(GL_TEXTURE_2D, FloorTexture);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);

		glTexCoord2f (0,0); glVertex3f (fCenterX-5, fCenterY,fCenterZ+5);
		glTexCoord2f (10,0); glVertex3f (fCenterX+5,fCenterY,fCenterZ+5);
		glTexCoord2f (10,10); glVertex3f (fCenterX+5,fCenterY,fCenterZ-5);
		glTexCoord2f (0,10); glVertex3f (fCenterX-5,fCenterY,fCenterZ-5);

	glEnd();
}

void scene_timer ()
{
	//angle += .5;	
	//glutTimerFunc (100, scene_timer, 0);
	point += Rot3 (Vec3(0,1,0),angle/180*vl_pi) * Vec3(1,0,0)*speed/10;	
	//pose_fsm();

	//glutPostRedisplay();
}
void yusi_special (unsigned char key)
{
#define CLAMP(v,lo,hi)  ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
	switch (key) {
		case GLUT_KEY_UP:
			speed += 0.1;
		break;
		case GLUT_KEY_DOWN:
			speed -= 0.1;
		break;
		case GLUT_KEY_RIGHT:
			angle -= 2;
		break;
		case GLUT_KEY_LEFT:
			angle += 2;
		break;
	}
	speed = CLAMP (speed, 0.0, 3.0);
}
void scene_keys (unsigned char key)
{
    switch (key) {
	case 'x':
		lightPosition[0] -= 0.1;
		break;
	case 'X':
		lightPosition[0] += 0.1;
		break;
    }
	if (key == ' ') {  // stand still
		speed = 0.0;
	} else if (key == 'j') {
		if (pose != 6) {   // for jump initialization
			MD2MSetLoop (yoshi, GL_FALSE); 
			oldpose = pose;  // save the current pose: stand or walk
			pose = 6;
			MD2MSetAnimation (yoshi, pose);
		}
		hit = Grab (point);
		cout << "hit: " << hit << endl;
		if (hit >= 0) {
			teapots[hit].status = 0;
			total_score += teapots[hit].score;
		}

	}
}

void get_scene_light_pos (float* pos)
{
    for (int i = 0; i < 4; i++)
	pos[i] = lightPosition[i];
}
