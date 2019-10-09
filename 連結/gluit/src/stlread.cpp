#include <stdio.h>
#include <stdlib.h>

#include "stlload.h"
#include <gl/glut.h>

GLuint stlRender2DL (const STLmodel& facet)
{
	GLuint dl = glGenLists (1);

	glNewList (dl, GL_COMPILE);

	glBegin (GL_TRIANGLES);
	// flat shaded
	for (int i = 0; i < facet.size(); i++) {
		glNormal3fv (facet[i].normal);
		glVertex3fv (facet[i].vertex1);
		glVertex3fv (facet[i].vertex2);
		glVertex3fv (facet[i].vertex3);
	}
	glEnd();

	glEndList ();
	return dl;
}

void stlUnitize (STLmodel &facet)
{
	// find min, max
	float min[3], max[3];
	int i, j; 

	min[0] = min[1] = min[2] = 1e10;
	max[0] = max[1] = max[2] = -1e10;
	int num = facet.size();
	
	for (i = 0; i < num; i++) {
		for (j = 0; j < 3; j++) {
			if (facet[i].vertex1[j] < min[j]) min[j] = facet[i].vertex1[j]; 
			if (facet[i].vertex1[j] > max[j]) max[j] = facet[i].vertex1[j]; 

			if (facet[i].vertex2[j] < min[j]) min[j] = facet[i].vertex2[j]; 
			if (facet[i].vertex2[j] > max[j]) max[j] = facet[i].vertex2[j]; 
			
			if (facet[i].vertex3[j] < min[j]) min[j] = facet[i].vertex3[j]; 
			if (facet[i].vertex3[j] > max[j]) max[j] = facet[i].vertex3[j]; 
		}
	}

	float center[3];
	for (j = 0; j < 3; j++) center[j] = (min[j]+max[j])/2;
#define Max(x,y) ((x)>(y)? (x): (y))
	float size = Max(Max(max[2]-min[2], max[1]-min[1]), max[0]-min[0]);

	for (i = 0; i < num; i++) {
		for (j = 0; j < 3; j++) {
			facet[i].vertex1[j] -= center[j];
			facet[i].vertex1[j] /= (size/2);

			facet[i].vertex2[j] -= center[j];
			facet[i].vertex2[j] /= (size/2);
			
			facet[i].vertex3[j] -= center[j];
			facet[i].vertex3[j] /= (size/2);
		}
	}

}

// not used
void stlFree (STLmodel &model)
{
	model.clear();
}

// From:
// http://forums.codeguru.com/showthread.php?148668-loading-a-stl-3d-model-file
//
STLmodel stlRead (char* STLfile)
{
	/////////////////////////stl loader..............
	FILE *binary;

	if( (binary = fopen(STLfile,"rb"))==NULL )////stl file location
	{
		printf(" \n the is failed to accessed");
		exit(1);
	}

	char filename[80];
	int numfacet[1];
	
	fread(filename, sizeof(char),80,binary) ;
	fread(numfacet, sizeof(int),1,binary);

	int num=numfacet[0];

	vector<FACET> model; 
	//FACET *facet = (FACET*) malloc (sizeof (FACET)*num);

	int j;
	FACET facet;
	for (j=0;j<num;j++)
	{
		fread(facet.normal,sizeof(float),3,binary);
		fread(facet.vertex1,sizeof(float),3,binary);
		fread(facet.vertex2,sizeof(float),3,binary);
		fread(facet.vertex3,sizeof(float),3,binary);
		fread(facet.unuse,sizeof(char),2,binary);
		model.push_back (facet);
	}

	//////////////////////stl loader

	return model;
}


