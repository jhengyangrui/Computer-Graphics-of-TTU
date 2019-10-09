#include <stdio.h>
#include <gl/glew.h>
#include <GL/glut.h>
#include "gl/glpng.h"
#include "gluit.h"
#include "svl/svl.h"
#include "md2m.h"
#include "teapot.h"
#include <math.h>


static int brickTid;
static int texedobjloc;

static GLuint FloorTexture;
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
/*void initFloor(char* filename)
{
	pngSetStandardOrientation (1);
	glActiveTexture (GL_TEXTURE0);
	FloorTexture = pngBind(filename, PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	
	int sceneMaploc = glGetUniformLocation (program, "sceneMap");
	glUniform1i (sceneMaploc, 0);  // scene use TU #0

	texedobjloc = glGetUniformLocation (program, "textured_object"); 
}*/
void initScene(int program)
{
	/*glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable (GL_COLOR_MATERIAL);

	pngSetStandardOrientation(1);
	glActiveTexture (GL_TEXTURE0);  // TU #0
	brickTid = pngBind("brick.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_NEAREST, GL_NEAREST);

	int sceneMaploc = glGetUniformLocation (program, "sceneMap");
	glUniform1i (sceneMaploc, 0);  // scene use TU #0

	texedobjloc = glGetUniformLocation (program, "textured_object");  // texture_object == 1 for textured object*/
	double h = 2.0; // height of teapot
	teapots[0].pos = Vec3 (2,h,3); teapots[0].score = 10;
	teapots[1].pos = Vec3 (0,h,0); teapots[1].score = 10;
	teapots[2].pos = Vec3 (2,h,-3); teapots[2].score = 10;
	init_md2();

	pngSetStandardOrientation (1);
	glActiveTexture (GL_TEXTURE0);
	FloorTexture = pngBind("floor.png", PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	int sceneMaploc = glGetUniformLocation (program, "sceneMap");
	glUniform1i (sceneMaploc, 0);  // scene use TU #0

	texedobjloc = glGetUniformLocation (program, "textured_object"); 
}
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
//------------------------------------------------------------------
void DrawFloor(float fCenterX, float fCenterY, float fCenterZ)
{
	float FloorColor[] = {1,1,1,0.6};
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, FloorColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);


	// set up blending so we can see the reflected cube through the
	// surface, and thus create the illusion of reflection
	glActiveTexture (GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FloorTexture);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);

		glTexCoord2f (0,0); glVertex3f (fCenterX-5, fCenterY,fCenterZ+5);
		glTexCoord2f (10,0); glVertex3f (fCenterX+5,fCenterY,fCenterZ+5);
		glTexCoord2f (10,10); glVertex3f (fCenterX+5,fCenterY,fCenterZ-5);
		glTexCoord2f (0,10); glVertex3f (fCenterX-5,fCenterY,fCenterZ-5);

	glEnd();	
	glPushMatrix();
	//glUseProgram (0);
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);

		glTexCoord2f (0,0); glVertex3f (-5, +1,-2);
		glTexCoord2f (1,0); glVertex3f (+5,+1,-2);
		glTexCoord2f (1,1); glVertex3f (+5,0,-2);
		glTexCoord2f (0,1); glVertex3f (-5,0,-2);		
	glEnd();
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}

//-------------------------------------
static float angle = 0.0;
static int rotate = 1;
void scene_timer()
{
	//if (rotate) angle += 5.0;
	point += Rot3 (Vec3(0,1,0),angle/180*vl_pi) * Vec3(1,0,0)*speed/10;		
}

static float lightPosition[4] = {-1,3,3,1};
void get_scene_light_pos (float* pos)
{
	for (int i =0; i < 4; i++)
		pos[i] = lightPosition[i];
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
	case 'r':
		rotate ^= 1;
		break;
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

void DrawScene()
{
	/*//Display lists for objects
	static GLuint spheresList=0, torusList=0, baseList=0;

	//Create spheres list if necessary
	if(!spheresList)
	{
		spheresList=glGenLists(1);
		glNewList(spheresList, GL_COMPILE);
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glPushMatrix();
			glTranslatef(0.45f, 1.0f, 0.45f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(-0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.0f, 0.0f,-0.9f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glPopMatrix();
		}
		glEndList();
	}

	//Create torus if necessary
	if(!torusList)
	{
		torusList=glGenLists(1);
		glNewList(torusList, GL_COMPILE);
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glPushMatrix();

			glTranslatef(0.0f, 0.5f, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glutSolidTorus(0.2, 0.5, 24, 48);

			glPopMatrix();
		}
		glEndList();
	}

	//Create base if necessary
	if(!baseList)
	{
		baseList=glGenLists(1);
		glNewList(baseList, GL_COMPILE);
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin (GL_QUADS);
			glNormal3f (0,1,0);
			glMultiTexCoord2i (GL_TEXTURE0, 0,0); glVertex3i (3,0,3);
			glMultiTexCoord2i (GL_TEXTURE0, 1,0); glVertex3i (3,0,-3);
			glMultiTexCoord2i (GL_TEXTURE0, 1,1); glVertex3i (-3,0,-3);
			glMultiTexCoord2i (GL_TEXTURE0, 0,1); glVertex3i (-3,0,3);
			glEnd();
		}
		glEndList();
	}

	////////////////////////////////////////////////////////////
	// Scene Rendering 
	glUniform1i (texedobjloc, 1);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, brickTid);
	glCallList(baseList);

	glUniform1i (texedobjloc, 0);
	glCallList(torusList);
	
	glUniform1i (texedobjloc, 0);
	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glCallList(spheresList); 
	glPopMatrix();*/
	glUniform1i (texedobjloc, 1);
	DrawFloor (0,0,0);
	glPushMatrix();
	glTranslated (point[0], point[1],point[2]);
	glRotatef (angle, 0,1,0);

	glTranslatef (0,1,0);
	glScalef (2/scale, 2/scale, 2/scale);
	glTranslatef (-center[0],-center[1],-center[2]);
	//glDisable (GL_LIGHTING);
	loopover = MD2MInterpolate (yoshi);
	glPopMatrix();

	glUniform1i (texedobjloc, 0);
	// teapots
	for (int i = 0; i < nteapots; i++) {
		teapots[i].show();
	}
	//glUseProgram (0);
	BEGIN_2D_OVERLAY (10,10);
	glDisable (GL_TEXTURE_2D);
	glColor3f (1,1,0);
	char display[20];
	sprintf (display, "Score: %d", total_score);
	drawstr (1,1, display);
	END_2D_OVERLAY();

}

