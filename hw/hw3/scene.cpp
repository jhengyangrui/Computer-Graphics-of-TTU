//#include <gl/glut.h>
#include "gluit.h"
#include "svl/svl.h"

#if 0	GLMmodel *m1;
	// reading OBJ model
	m1 = glmReadOBJ ("pickup.obj");
	glmUnitize (m1);
	glmFacetNormals (m1);
	glmVertexNormals (m1, 90);


//////////////////////////////////////////////////////
GLuint treetexture;

	pngSetStandardOrientation(1); // important to set "before" loading
	// remember set to PNG_ALPHA to read in the alpha channel
	// set to GL_LINEAR (better than GL_NEAREST)
	treetexture = pngBind("shrub1.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
#endif






//////////////////////////////////////////////////////
void grid()
{
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10, 0, i), glVertex3i (10,0,i);
		glVertex3i (i,0,-10), glVertex3i (i,0,10);
	}
	glEnd();
}
//////////////////////////////////////////////////////

void drawcar()
{
extern GLMmodel *m1;
	glmDraw (m1, GLM_SMOOTH|GLM_MATERIAL);
}
	

//
// size can be set to Vec3(1,2,1) grow a taller tree
//
void drawXtree (const Vec3& center, const Vec3& size)
{
    glPushAttrib (GL_ALL_ATTRIB_BITS);
extern GLuint treetexture;
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, treetexture);
	glEnable (GL_ALPHA_TEST);
	glDisable (GL_CULL_FACE);
	glAlphaFunc (GL_GREATER, 0);

	glPushMatrix();
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
