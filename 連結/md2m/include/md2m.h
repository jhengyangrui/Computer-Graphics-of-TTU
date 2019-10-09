#include "md2_main.h"
#include "md2.h"


#ifdef _MSC_VER
	#pragma comment (lib, "md2m.lib")
//	#pragma comment (lib, "glaux.lib")
#endif

// function declaration

extern t3DModel* MD2MReadMD2 (char*, char*);
extern void MD2MLoad (t3DModel*, char*, char*);
extern void MD2MSetViewMode (t3DModel*,int);
extern void MD2MSetAnimationSpeed (t3DModel*,float);
extern void MD2MSetAnimation (t3DModel*, int);
extern void MD2MSetLoop (t3DModel*, GLboolean);
extern void MD2MFindDimension (t3DModel*, float*, float*);
extern int  MD2MInterpolate (t3DModel*);
