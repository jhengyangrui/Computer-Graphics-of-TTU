
#include <stdlib.h>

//#include <gl/glut.h>

#include "md2_main.h"
#include "Md2.h"

#include <windows.h>

// This stores the speed of the animation between each key frame
//static float kAnimationSpeed = 5.0f;

static UINT g_Texture[MAX_TEXTURES] = {0};		// This holds the texture info, referenced by an ID

static CLoadMD2 g_LoadMd2;				// This is MD2 class.  This should go in a good model class.

enum {X=0, Y,Z};

// add the bound texture to t3Dmodel
// whenever display, bind the texture


void MD2MLoad (t3DModel *md2m, char *md2file, char* texfile)
{
	// Load the Quake2 model with its file name and texture name
	bool status = g_LoadMd2.ImportMD2(md2m, md2file, texfile);		
	if (status == false) {
		printf ("cannot open %s and/or %s\n", md2file, texfile);
		exit(-1);
	}
		
	// Go through all the materials
	if (md2m->numOfMaterials > 1) {
		printf ("weird ...\n");
		getchar();
		exit (-1);
	}

	// setting default animation speed and viewmode
	md2m->kAnimationSpeed = 5.0;
	md2m->viewMode = GL_FILL;
	md2m->loop = GL_TRUE;
	md2m->stayEnd = GL_FALSE;
    md2m->lastTime = 0;

	for(int i = 0; i < md2m->numOfMaterials; i++)
	{
		// Check to see if there is a file name to load in this material
		if(strlen(md2m->pMaterials[i].strFile) > 0)
		{
			// Use the name of the texture file to load the bitmap, with a texture ID (i).
			// We pass in our global texture array, the name of the texture, and an ID to reference it.	
			CreatePNGTexture(g_Texture, md2m->pMaterials[i].strFile, i);			
		}

		// Set the texture ID for this material
		md2m->pMaterials[i].textureId = g_Texture[i];//i;
	}

	// To make our model render somewhat faster, we do some front back culling.
	// It seems that Quake2 orders their polygons clock-wise.

//	glEnable(GL_CULL_FACE);								// Turn culling on
//	glCullFace(GL_FRONT);								// Quake2 uses front face culling apparently
}

t3DModel* MD2MReadMD2 (char *md2file, char* texfile)
{
	t3DModel *mymodel = (t3DModel*) malloc (sizeof (t3DModel));
	MD2MLoad (mymodel, md2file, texfile);
	return mymodel;
}

void MD2MSetViewMode (t3DModel *md2m, int viewmode)
{
    md2m->viewMode = viewmode;
}

void MD2MSetAnimationSpeed (t3DModel *md2m, float speed)
{
	md2m->kAnimationSpeed = speed;
}

void MD2MFindDimension (t3DModel *pModel, float *center, float* size)
{
//#define MAX(x,y) ((x) > (y)?(x): (y))    
    // based on first frame
	// Get the first key frame so we have an address to the texture and face information
    t3DObject *pObject = &pModel->pObject[0];
    float max[3], min[3];
    
    max[X]=max[Y]=max[Z] = -1e5;
    min[X]=min[Y]=min[Z] = 1e5;
    
    // find min/max
    for (int i = 0; i < pObject->numOfVerts; i++) {
    	CVector3 point = pObject->pVerts[i];
    	if (point.x > max[X]) max[X] = point.x;
    	if (point.x < min[X]) min[X] = point.x;
    	if (point.y > max[Y]) max[Y] = point.y;
    	if (point.y < min[Y]) min[Y] = point.y;
    	if (point.z > max[Z]) max[Z] = point.z;
    	if (point.z < min[Z]) min[Z] = point.z;
    }

  /* calculate model width, height, and depth */
    size[X] = max[X] - min[X];
    size[Y] = max[Y] - min[Y];
    size[Z] = max[Z] - min[Z];

  /* calculate center of the model */
    center[X] = (max[X] + min[X]) / 2.0;
    center[Y] = (max[Y] + min[Y]) / 2.0;
    center[Z] = (max[Z] + min[Z]) / 2.0;

	// instead of modifying the verts (you'll need to do so for EVERY key)
	// just return the center and scale
}

//
//
// should be separate for each model ... [in pModel->lastTime]
// otherwise, the animation is not smooth (delta time got mixed up)
//
static float ReturnCurrentTime(t3DModel *pModel, int nextFrame)
{
	DWORD elapsedTime   = 0;
// DWORD: unsigned long;

	// This function is very similar to finding the frames per second.
	// Instead of checking when we reach a second, we check if we reach
	// 1 second / our animation speed. (1000 ms / kAnimationSpeed).
	// That's how we know when we need to switch to the next key frame.
	// In the process, we get the t value for how we are at to going to the
	// next animation key frame.  We use time to do the interpolation, that way
	// it runs the same speed on any persons computer, regardless of their specs.
	// It might look chopier on a junky computer, but the key frames still be
	// changing the same time as the other persons, it will just be not as smooth
	// of a transition between each frame.  The more frames per second we get, the
	// smoother the animation will be.

	// Get the current time in milliseconds
	DWORD time = GetTickCount();

	// Find the time that has elapsed since the last time that was stored
	elapsedTime = time - pModel->lastTime;

	// To find the current t we divide the elapsed time by the ratio of 1 second / our anim speed.
	// Since we aren't using 1 second as our t = 1, we need to divide the speed by 1000
	// milliseconds to get our new ratio, which is a 5th of a second.
	float t = elapsedTime / (1000.0f / pModel->kAnimationSpeed);
	//printf ("t: %f\n", t);	
	
	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if (elapsedTime >= (1000.0f / pModel->kAnimationSpeed) ) // deltaT/1000 > 1/Hz
	{
		// Set our current frame to the next key frame (which could be the start of the anim)
		pModel->currentFrame = nextFrame;
//printf ("current: %d\n", pModel->currentFrame);
		// Set our last time to the current time just like we would when getting our FPS.
		pModel->lastTime = time;
		return 0.0;    // meaning: start of nextFrame
		
// (4/13/05): [NY] for even slower machines, may need to advance more than one frames		
	}

	// Return the time t so we can plug this into our interpolation.

#if 0	
	// (fix): 3/14
	// if the system got delayed so much, t might > 0
	// (the interpolation then becomes extrapolation)
	// (sometimes the figure appears huge).
	// in that case, set t = 0.5
	if (t > 1.0) t = 0.5;
#endif

	return t;   // fraction: [0,1]
}

//
// set which animation to run
//
void MD2MSetAnimation (t3DModel *pModel, int whichAnim)
{
	pModel->currentAnim = whichAnim;

	// Set the current frame to be the starting frame of the new animation
	pModel->currentFrame = pModel->pAnimations[whichAnim].startFrame;
	
	// (4/13/05): record the timestamp of start of this pose
	pModel->lastTime = GetTickCount();

	// Make sure we have valid objects just in case. (size() is in the vector class)
	if(pModel->pObject.size() <= 0) return;

//printf ("pAnim %s: [%d,%d]\n", pAnim->strName, pAnim->startFrame, pAnim->endFrame);

}


// some actions are not supposed to be looped (e.g., dead, jump,...)
void MD2MSetLoop (t3DModel *pModel, GLboolean value)
{
    // gl.h: GL_TRUE (1) GL_FALSE (0)
    if (value == GL_TRUE) { 
		pModel->loop = GL_TRUE;
		pModel->stayEnd = GL_FALSE;    // reset to default value (if it was ever set true)
	} else {
		pModel->loop = GL_FALSE;
//		gStayEnd = GL_TRUE;     ... will be set to TRUE after the loop is over
	}
}

// how to stay dead remain at end frame....
// remove MD2M from scene
void MD2MRemove (t3DModel *pModel)
{
    // effect: blend off
}

//
// to be called in each display loop
// also, notify the caller when the animation has finished one loop
//
int MD2MInterpolate (t3DModel *pModel)
{
	int loopOver = 0;

	// Here we grab the current animation that we are on from our model's animation list
	tAnimationInfo *pAnim = &(pModel->pAnimations[pModel->currentAnim]);

	// (currentFrame might change in time)

	// This gives us the current frame we are on.  We mod the current frame plus
	// 1 by the current animations end frame to make sure the next frame is valid.
	// If the next frame is past our end frame, then we go back to zero.  We check this next.
	int nextFrame = (pModel->currentFrame + 1) % pAnim->endFrame;
//printf ("[%d %d]\n", pModel->currentFrame, nextFrame);

	// If the next frame is zero, that means that we need to start the animation over.
	// To do this, we set nextFrame to the starting frame of this animation.
	if(nextFrame == 0) { 
		nextFrame =  pAnim->startFrame;
        loopOver = 1;
        // however, if the looping was turned off, stay at the endframe 
        if (pModel->loop == GL_FALSE)
			pModel->stayEnd = GL_TRUE;
	}

	// (4/13/05) get the fraction
	// note that the currentFrame might be advanced if the elapsed time is larger than 1/Speed
	//
	float t = ReturnCurrentTime(pModel, nextFrame);

	// Get the current key frame we are on
	t3DObject *pFrame =		 &pModel->pObject[pModel->currentFrame];

	// Get the next key frame we are interpolating too
	// (4/13/05): note even if the frame advanced, nextFrame == currentFrame
	// as we have designed: will show the frame (no interpolation)
	t3DObject *pNextFrame =  &pModel->pObject[nextFrame];

	// Get the first key frame so we have an address to the texture and face information
	t3DObject *pFirstFrame = &pModel->pObject[0];

	// Next, we want to get the current time that we are interpolating by.  Remember,
	// if t = 0 then we are at the beginning of the animation, where if t = 1 we are at the end.
	// Anything from 0 to 1 can be thought of as a percentage from 0 to 100 percent complete.
//printf ("[%d %d]\n", pModel->currentFrame, nextFrame);

	if (pModel->stayEnd == GL_TRUE) {
		pFrame =  &pModel->pObject[pAnim->endFrame-1];  // endFrame actually points to the start of next anim
		pNextFrame =&pModel->pObject[pAnim->endFrame-1];
	}
//printf ("[%x] %x %x\n", pModel, pFrame, pNextFrame);


	// save current texture 
	int tex2dbound;
	glGetIntegerv (GL_TEXTURE_BINDING_2D, &tex2dbound);
	
	// assume the model has one texture ...
	glBindTexture (GL_TEXTURE_2D, pModel->pMaterials[0].textureId);
	// Start rendering lines or triangles, depending on our current rendering mode (Lft Mouse Btn)
	
	glPolygonMode (GL_FRONT_AND_BACK, pModel->viewMode);
	glBegin(GL_TRIANGLES);

		// Go through all of the faces (polygons) of the current frame and draw them
		for(int j = 0; j < pFrame->numOfFaces; j++)
		{
			// Go through each corner of the triangle and draw it.
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];

				// Get the index for each texture coordinate for this face
				int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
						
				// Make sure there was a UVW map applied to the object.  Notice that
				// we use the first frame to check if we have texture coordinates because
				// none of the other frames hold this information, just the first by design.
				if(pFirstFrame->pTexVerts) 
				{
					// Pass in the texture coordinate for this vertex
					glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
								 pFirstFrame->pTexVerts[ texIndex ].y);
				}

				// Now we get to the interpolation part! (*Bites his nails*)
				// Below, we first store the vertex we are working on for the current
				// frame and the frame we are interpolating too.  Next, we use the
				// linear interpolation equation to smoothly transition from one
				// key frame to the next.
				
				// Store the current and next frame's vertex
				CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
				CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];

				// By using the equation: p(t) = p0 + t(p1 - p0), with a time t
				// passed in, we create a new vertex that is closer to the next key frame.
				glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), // Find the interpolated X
						   vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
						   vPoint1.z + t * (vPoint2.z - vPoint1.z));// Find the interpolated Z
			}
		}

	glEnd();	

	glBindTexture (GL_TEXTURE_2D, tex2dbound);
    
	return loopOver;	
}


