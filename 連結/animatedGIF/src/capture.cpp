#include <stdlib.h>
#include <gl/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

//由於TGA2GIF轉檔程式，由下往上讀取，因此改為上下相反儲存tga圖檔
static int ww, wh;
static int in_capture;
static int images;

extern void makeGIF (const char*,int,int);
extern "C" void tga2gif(char *filename);
extern "C" int tgaSave(char*,short,short,unsigned char,unsigned char*);

static char* tgahead = "frame";

static void getframe(const char* tgahead)
{	
	unsigned char *imageData,*tempdata;
	imageData = (unsigned char *)malloc(sizeof(unsigned char) * ww * wh * 3);
	tempdata = (unsigned char *)malloc(sizeof(unsigned char) * ww * 1 * 3);
	int i=0;
//glReadBuffer (GL_FRONT);
	for(int j=wh-1; j>=0; j--){
		glReadPixels(0,j,ww,1,GL_RGB,GL_UNSIGNED_BYTE, (GLvoid *)tempdata);
		int k=0;
		while(k<ww*3){
			imageData[i] = tempdata[k];
			i++;
			k++;
		}
	}
	char newFilename[50];
	sprintf(newFilename,"%s%d.tga",tgahead,images);
	tgaSave(newFilename,ww,wh,24,imageData);
	images++;

	free(imageData); 
	free(tempdata);
}



static void timer(int dummy)
{
	if (in_capture) {
		printf ("captured..\n");
		getframe("frame");
		glutTimerFunc (100, timer, 0);
	}
}


static void processGIF (int image_count, int delaytime)
{
	char name[50];
	int index=0;
	while(index<image_count){
		sprintf(name,"%s%d",tgahead,index);
		tga2gif(name);//tga轉檔為gif
		index++;			
	}
	makeGIF("frame", image_count, delaytime);//合成GIF檔
	char tganame[50],gifname[50];

	index=0;
	while(index<image_count){//刪除擷取的檔案
		sprintf(tganame,"%s%d.tga",tgahead,index);
		sprintf(gifname,"%s%d.gif",tgahead,index);
//		DeleteFile((LPCWSTR)tganame);  // FOR VC9
		DeleteFile((LPSTR)tganame);  // FOR VC9
//		DeleteFile((LPCWSTR)gifname);
		DeleteFile((LPSTR)gifname);
		index++;			
	}
}

void ProcessCapture (int window, int delaytime=10)
{
	int curr_win = glutGetWindow ();
	glutSetWindow (window);
	ww = glutGet (GLUT_WINDOW_WIDTH);
	wh = glutGet (GLUT_WINDOW_HEIGHT);

	if (in_capture == 0) { // start capturing	
		glutTimerFunc (100, timer, 0);
		in_capture = 1;
	} else if (in_capture == 1){ // stop capturing
		in_capture = 0;
		processGIF (images, delaytime);
		images = 0;
	}
	glutSetWindow (curr_win);
}
