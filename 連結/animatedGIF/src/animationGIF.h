#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>
#include <time.h>
#include "tga.h"
#include <math.h>
#include <windows.h>

#pragma pack(push,gifpacking,1)


typedef struct {
	char cSignature[3]; // Must be 'GIF'
	char cVersion[3];   // Must be '89a'
} GIF_HEADER;

typedef struct { // 7 bytes
	unsigned short iWidth;
	unsigned short iHeight;
	unsigned char iSizeOfGct : 3;
	unsigned char iSortFlag : 1;
	unsigned char iColorResolution : 3;
	unsigned char iGctFlag : 1;
	unsigned char iBackgroundColorIndex;
	unsigned char iPixelAspectRatio;
} GIF_LOGICAL_SCREEN_DESCRIPTOR;

typedef struct { // 6 bytes
	unsigned char iBlockSize;           // Must be 4
	unsigned char iTransparentColorFlag : 1;
	unsigned char iUserInputFlag : 1;
	unsigned char iDisposalMethod : 3;
	unsigned char iReserved : 3;
	unsigned short iDelayTime;
	unsigned char iTransparentColorIndex;
	unsigned char iBlockTerminator;     // Must be 0
} GIF_GRAPHIC_CONTROL_EXTENSION;

typedef struct { // 9 bytes
	unsigned short iLeft;
	unsigned short iTop;
	unsigned short iWidth;
	unsigned short iHeight;
	unsigned char iSizeOfLct : 3;
	unsigned char iReserved : 2;
	unsigned char iSortFlag : 1;
	unsigned char iInterlaceFlag : 1;
	unsigned char iLctFlag : 1;
} GIF_IMAGE_DESCRIPTOR;

#pragma pack(pop,gifpacking)

unsigned short iGctSize[]={6,12,24,48,96,192,384,768};