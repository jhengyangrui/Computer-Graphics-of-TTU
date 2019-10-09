#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"
#include "svl/svl.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "svl-dbg.lib")

int mainwin, viewer;
float theta = 0;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

GLuint p;

GLuint p1, colortex, normaltex;
GLuint tan_loc, bitan_loc;
GLuint alphatex, walltex, floortex;
GLuint nobumploc;
int bumptoggle;

void grid()
{
	/*glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10,0,i), glVertex3i (10,0,i);
		glVertex3i (i,0,-10), glVertex3i (i,0,10);
	}
	glEnd();*/	
	glBegin (GL_QUADS);
		// same attributes for every vertex
		glVertexAttrib3f(tan_loc,1,0,0);
		glVertexAttrib3f(bitan_loc, 0,1,0);
		//glNormal3f (0,1,0);
		
		glMultiTexCoord2f (GL_TEXTURE0, 0,0); glVertex3i (-10, 0, -10); 
		glMultiTexCoord2f (GL_TEXTURE0, 1,0); glVertex3i (10,0,-10);
		glMultiTexCoord2f (GL_TEXTURE0, 1,1); glVertex3i (10,0,10);
		glMultiTexCoord2f (GL_TEXTURE0, 0,1); glVertex3i (-10,0,10);
	glEnd();
}
void wall()
{
	glBegin (GL_QUADS);
		glMultiTexCoord2f (GL_TEXTURE0, 0,0); glVertex3i (-2, 0, 0); 
		glMultiTexCoord2f (GL_TEXTURE0, 1,0); glVertex3i (2,0,0);
		glMultiTexCoord2f (GL_TEXTURE0, 1,1); glVertex3i (2,4,0);
		glMultiTexCoord2f (GL_TEXTURE0, 0,1); glVertex3i (-2,4,0);
	glEnd();
}

GLuint holeflag;

void content()
{
	
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glUseProgram(0);
	Vec3 lightpos = Vec3 (-2, 2, 2);
	Vec3 lpos = Rot3 (Vec3 (0, 1, 0), theta) * lightpos;
	float pos[4];
	pos[0] = lpos[0];
	pos[1] = lpos[1];
	pos[2] = lpos[2];
	pos[3] = 1.0;

	glLightfv (GL_LIGHT0, GL_POSITION, pos);

	/*float white[] = {1,1,1,1};
	glLightfv (GL_LIGHT0, GL_AMBIENT, white);*/
	
	glUseProgram(p1);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, normaltex);
	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, colortex);
	grid();

	//glUseProgram(0);

	glUseProgram(p);
	
	glActiveTexture (GL_TEXTURE2);
	glBindTexture (GL_TEXTURE_2D, walltex);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, alphatex);
	glUniform1i (holeflag, 0);
	wall();
	glUniform1i (holeflag, 1);

	glPushMatrix();
	glTranslatef (2,0,2);
	glRotatef (-90, 0,1,0);
	wall();
	glPopMatrix();

	glUseProgram(0);
	glPushAttrib (GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glColor3ub (255,255,0); 
	glTranslatef (pos[0],pos[1],pos[2]);
	glutWireSphere(0.2, 20, 20);

	glutSwapBuffers();
}

int animate = 0;
void init()
{
	glEnable (GL_DEPTH_TEST);
	glClearColor (.4,.4,.4,1);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glewInit(); 

	extern GLuint setShaders(char*,char*);
	p = setShaders ("hole.vert", "hole.frag");
	extern GLuint setShaders(char*,char*);
	p1 = setShaders ("bumpmap.vert", "bumpmap.frag");
	// set texture object in each texture unit

	pngSetStandardOrientation(1);
	walltex = pngBind("brick.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	alphatex = pngBind("alphamask.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	colortex = pngBind("rock.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	normaltex = pngBind("rockDOT3.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	//floortex = pngBind("floor.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_REPEAT, GL_NEAREST, GL_NEAREST);

	
	//glewInit();
	//pngSetStandardOrientation(1);
	//colortex = pngBind("rock.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	//normaltex = pngBind("rockDOT3.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	glUseProgram (p1);
	glUniform1i (glGetUniformLocation (p1,"NormalMap"), 0);
	glUniform1i (glGetUniformLocation (p1,"ColorMap"), 1);
	glUniform1i (glGetUniformLocation (p1,"nobump"), animate);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, normaltex);
	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, colortex);
	
	tan_loc = glGetAttribLocation (p1,"Tangent");
	bitan_loc = glGetAttribLocation (p1,"Bitangent");
	//nobumploc = glGetUniformLocation (p1,"nobump");

	// weird ... TU0 & TU1
	glActiveTexture (GL_TEXTURE2);
	glBindTexture (GL_TEXTURE_2D, walltex);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, alphatex);

	glUseProgram(p);  //// key .....
	// set uniforms
	glUniform1i (glGetUniformLocation (p, "wallmap"), 2);
	glUniform1i (glGetUniformLocation (p, "alphamap"), 0);
	holeflag = glGetUniformLocation (p, "dighole");

}
float a = 0;
void timer (int dummy)
{
	theta = theta + (0.1 * a);
	if (theta <= 0.0){
		a = 1;printf("1");
	}
	if (theta >= 2){
		a = -1;printf("-1");
	}
	glutPostRedisplay();
	glutTimerFunc (100, timer, 0);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case ' ': // switch
		animate ^= 1;

		glUseProgram (p1);//printf("-1000");
		glUniform1i (glGetUniformLocation (p1,"nobump"), animate);
		break;
	}
}
void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("dig a hole");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	ViewerExtent (viewer, 10.0);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc (100, timer, 0);
	glutMainLoop();
}
