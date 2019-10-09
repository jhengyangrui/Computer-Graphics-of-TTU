#include <gl/glut.h>
#include "gl/glpng.h"
#include "vect.h"

static int tree_texture;


void inittreetexture(void)
{
    pngInfo info;

	pngSetStencil (0,0,0);
	pngSetStandardOrientation (1);
	tree_texture = pngBind ("tree.png", PNG_NOMIPMAP, PNG_STENCIL, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
}

static void GetViewPt (Vector viewpt) 
{
    float m[16];
	// get the viewpoint
	glGetFloatv (GL_MODELVIEW_MATRIX, m);
    viewpt[0] = -m[0]*m[12]-m[1]*m[13]-m[2]*m[14];   // Rx+t = 0
    viewpt[1] = -m[4]*m[12]-m[5]*m[13]-m[6]*m[14];
    viewpt[2] = -m[8]*m[12]-m[9]*m[13]-m[10]*m[14];
//printf ("viewpt: [%f %f %f]\n", viewpt[0],viewpt[1],viewpt[2]);
}

void drawBillboard (Vector center)
{
	VECTOR n, b, localx, localy, p, py, pxz, rot1, rot2;
	double phi, theta;
    VECTOR viewpt;

    GetViewPt (viewpt);
	
    // billboard
    CopyVec (center, b);
	SetVec (0.,0.,1., n);   // pointing +z
	UnitArrowVec (b, viewpt, p);
	SetVec (0.,p[1],0., py);
	SubVecTo (p, py, pxz); NormVec (pxz);
    SetVec (0.,1.,0., localy);
	
	// rotate local y
	UnitCrossVec (n, pxz, rot1); theta = acos (Min(1.0,VecDot (n, pxz)));
	if (VecDot (rot1, localy) < 0)
		theta = -theta;
#if 0
	   // rotate local x
       CrossVec (localy, pxz, localx);
	   UnitCrossVec (pxz, p, rot2); phi = acos (Min(1.0,VecDot (pxz,p)));
       // sometimes floating point yield a value slightly more than 1.0, causing acos to
	   // return bogus
	   if (VecDot (rot2, localx) < 0)
	      phi = -phi;
#endif
    glPushAttrib (GL_ALL_ATTRIB_BITS);
	   
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, tree_texture);
	glDisable (GL_CULL_FACE);
	glEnable (GL_ALPHA_TEST);

	glAlphaFunc (GL_GREATER, 0);
	glPushMatrix();
	    glTranslated (center[0], center[1], center[2]);
	    glRotatef (theta*180/PI, 0.,1.,0.);
		//glRotatef (phi*180/PI, 1.,0.,0.);
   
		glBegin (GL_QUADS);
	    glTexCoord2f (0,0), glVertex2i (-1, 0);
	    glTexCoord2f (1,0), glVertex2i (1, 0);
	    glTexCoord2f (1,1), glVertex2i (1, 2);
	    glTexCoord2f (0,1), glVertex2i (-1, 2);
	    glEnd();
    glPopMatrix();
    
	glPopAttrib();
}

void drawXtree (Vector center)
{
    glPushAttrib (GL_ALL_ATTRIB_BITS);

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, tree_texture);
	glEnable (GL_ALPHA_TEST);
	glDisable (GL_CULL_FACE);
	glAlphaFunc (GL_GREATER, 0);
	glPushMatrix();
	    glTranslated (center[0], center[1], center[2]);
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
