#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"
#include <svl/svl.h>

#pragma comment (lib, "glew32.lib")

GLuint prog;
GLMmodel *toycar;
GLuint texName;
int mainwin, viewer;
void display(){}

void grid()
{
	glLineWidth (1.0);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10,0,i); glVertex3i (10,0,i);
		glVertex3i (i,0,-10); glVertex3i (i,0,10);
	}
	glEnd();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

double theta;
//GLuint id;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	//float pos[4] = {1, 1, 1, 1};

	Vec3 lightpos = Vec3 (1, 1, 1);
	Vec3 lpos = Rot3 (Vec3 (0, 1, 0), theta) * lightpos;
	float pos[4];
	pos[0] = lpos[0];
	pos[1] = lpos[1];
	pos[2] = lpos[2];
	pos[3] = 1.0;

	glLightfv (GL_LIGHT0, GL_POSITION, pos);

	float white[] = {1,1,1,1};
	glLightfv (GL_LIGHT0, GL_AMBIENT, white);
	
	glUseProgram (prog);
	glmDraw(toycar, GLM_MATERIAL | GLM_SMOOTH); 

	glUseProgram (0);
	grid();
	glPushMatrix();
	glTranslatef (pos[0],pos[1],pos[2]);
	glutWireSphere(0.2, 20, 20);

	glutSwapBuffers();
}

#define checkImageHeight 64
#define checkImageWidth  64

GLubyte checkImage[checkImageWidth][checkImageHeight][4];
void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

GLuint tex_checker,tex_1000;

void init()
{
	glEnable (GL_DEPTH_TEST);
	glClearColor (0.4, 0.4, 0.4, 1);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glEnable (GL_TEXTURE_2D);

	
   makeCheckImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, & tex_checker);
   glBindTexture(GL_TEXTURE_2D,  tex_checker);  // bind to Texture Unit #0

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
#if 1
	pngSetStandardOrientation(1);
	 tex_1000 = pngBind("pond1.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_NEAREST, GL_NEAREST);
#endif*/
	glewInit();
	extern GLuint setShaders (char*, char*);
	prog = setShaders ("pointlight.vert", "pointlight.frag");

	glUseProgram (prog);
	int loc = glGetUniformLocation (prog, "tex");
	int loc2 = glGetUniformLocation (prog, "tex1");
	glUniform1i(loc,0);
	glUniform1i(loc2,1);

	toycar = glmReadOBJ ("toycar.OBJ");
	glmUnitize (toycar);
	glmFacetNormals (toycar);
	glmVertexNormals (toycar, 90.0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D,  tex_checker);
	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D,  tex_1000);

	glDisable (GL_LIGHTING);
	glActiveTexture(GL_TEXTURE0);
	glDisable (GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
}

int animate;

void timer (int dummy)
{
	if (animate){
		theta += 0.01;
	}
	glutPostRedisplay();
	glutTimerFunc (10, timer, 0);
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == ' '){
		animate ^= 1;
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	glutTimerFunc (10, timer, 0);
	glutKeyboardFunc (keyboard);
	glutMainLoop();
}
