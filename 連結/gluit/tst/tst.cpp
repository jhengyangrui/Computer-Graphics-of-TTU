#include "gluit.h"
int mainwin, viewer;

void display(void){}
void reshape(int w, int h)
{
	glViewport (0,0,w,h);
	ViewerReshape(viewer);
}

GLMmodel *pmodel,*pmodel_al,*pmodel_duck;
GLMmodel *pmodel_bug;

void content (void)
{
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glmDraw (pmodel, GLM_SMOOTH|GLM_MATERIAL);
#if 0
	glPushMatrix();
	glRotatef (-90,1,0,0);
	glmDraw (pmodel, GLM_SMOOTH|GLM_MATERIAL|GLM_TEXTURE);
	glPopMatrix();

	glTranslatef (-2,0,0);
	glmDraw (pmodel_duck, GLM_SMOOTH|GLM_MATERIAL|GLM_TEXTURE);
	
	glTranslatef (4,0,0);
	glmDraw (pmodel_al, GLM_SMOOTH|GLM_MATERIAL);
#endif
	glmDraw (pmodel_bug, GLM_SMOOTH|GLM_MATERIAL|GLM_TEXTURE);

	glutSwapBuffers();
}

void init()
{
#if 0
	pmodel_al = glmReadOBJ ("../OBJmodels/al.obj");
	pmodel = glmReadOBJ ("../OBJmodels/texOBJ/myafter/kitchen2.obj");
	pmodel_duck = glmReadOBJ ("../OBJmodels/texOBJ/rubberduck.obj");
#endif
	pmodel_bug = glmReadOBJ ("../OBJmodels/bug/Bench06.obj");
#if 0
	glmUnitize (pmodel);
	glmFacetNormals (pmodel);
	glmVertexNormals (pmodel, 90);
	
	glmUnitize (pmodel_al);
	glmFacetNormals (pmodel_al);
	glmVertexNormals (pmodel_al, 90);

	glmUnitize (pmodel_duck);
	glmFacetNormals (pmodel_duck);
	glmVertexNormals (pmodel_duck, 90);
#endif	
	glmUnitize (pmodel_bug);
	glmFacetNormals (pmodel_bug);
	glmVertexNormals (pmodel_bug, 90);

	GLfloat dimensions[3];
	glmDimensions (pmodel_bug, dimensions);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_DEPTH_TEST);
	glClearColor (.4,.4,.4,0);
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, 1);
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("textured OBJ");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();

	glutMainLoop();
}
