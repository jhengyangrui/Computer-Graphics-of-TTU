#include "animationGIF.h"

void makeGIF(const char *tgahead, int images, int delaytime)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];
	DWORD dw;
	char szColorTable[768];
	unsigned char c;
	HANDLE hFileOut, hFileIn;
	GIF_HEADER gh;
	GIF_LOGICAL_SCREEN_DESCRIPTOR glsd1, glsd;
	GIF_GRAPHIC_CONTROL_EXTENSION ggce;
	GIF_IMAGE_DESCRIPTOR gid;
	ZeroMemory(&glsd1,sizeof(GIF_LOGICAL_SCREEN_DESCRIPTOR));
	szFileName[0]=0;
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=NULL;
	ofn.hInstance=NULL;

	//	ofn.lpstrFilter=(LPWSTR) "GIF Files (*.gif)\0*.gif\0";
	ofn.lpstrFilter=(LPSTR) "GIF Files (*.gif)\0*.gif\0";

	ofn.lpstrCustomFilter=NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=1;

	//	ofn.lpstrFile=(LPWSTR)szFileName;
	ofn.lpstrFile=(LPSTR)szFileName;

	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;

//	ofn.lpstrTitle=(LPCWSTR)"Make Animated GIF";
	ofn.lpstrTitle=(LPSTR)"Make Animated GIF";
	
	ofn.Flags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
	ofn.nFileOffset=NULL;
	ofn.nFileExtension=NULL;

//	ofn.lpstrDefExt=(LPCWSTR)"gif";
	ofn.lpstrDefExt=(LPSTR)"gif";

	ofn.lCustData=NULL;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
	if (GetSaveFileName(&ofn)) {
//		hFileOut=CreateFile((LPCWSTR)szFileName,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,0,0);
		hFileOut=CreateFile((LPSTR)szFileName,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,0,0);

		strncpy((char *)&gh,"GIF89a",6);
		WriteFile(hFileOut,&gh,sizeof(GIF_HEADER),&dw,0);
		WriteFile(hFileOut,&glsd1,sizeof(GIF_LOGICAL_SCREEN_DESCRIPTOR),&dw,0);
		//if (IsDlgButtonChecked(hMainDlg,IDC_CHECK_MAIN_LOOP)) {
			WriteFile(hFileOut,"\41\377\013NETSCAPE2.0\003\001\377\377\0",19,&dw,0);
		//}
		//test = (unsigned)SendDlgItemMessage(hMainDlg,IDC_LIST_MAIN_FRAMES,LB_GETCOUNT,0,0);
		
		char *framename;
		framename = (char *)malloc(sizeof(char) * strlen(tgahead)+8);
		int index=0;

		while(index<images){			
		//for (dwIndex=0;dwIndex<images;dwIndex++) {
			ZeroMemory(&glsd,sizeof(GIF_LOGICAL_SCREEN_DESCRIPTOR));
			ZeroMemory(&ggce,sizeof(GIF_GRAPHIC_CONTROL_EXTENSION));
			ZeroMemory(&gid,sizeof(GIF_IMAGE_DESCRIPTOR));
			//char *intest;
			//intest = (char *)SendDlgItemMessage(hMainDlg,IDC_LIST_MAIN_FRAMES,LB_GETITEMDATA,dwIndex,0);
			sprintf(framename,"%s%d.gif",tgahead,index);
//			hFileIn=CreateFile((LPCWSTR)framename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0);
			hFileIn=CreateFile((LPSTR)framename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0);

			if (hFileIn==INVALID_HANDLE_VALUE) {
				//MessageBox(hMainDlg,"Could not open file. GIF creation aborted.",0,0);
				printf("Could not open file. GIF creation aborted.");
				CloseHandle(hFileOut);
				return;
			}
			ReadFile(hFileIn,&gh,sizeof(GIF_HEADER),&dw,0);
			if (strncmp(gh.cSignature,"GIF",3) || (strncmp(gh.cVersion,"89a",3) && strncmp(gh.cVersion,"87a",3))) {
				//MessageBox(hMainDlg,"Not a GIF file, or incorrect version number.",0,MB_ICONEXCLAMATION);
				printf("Not a GIF file, or incorrect version number.");
				CloseHandle(hFileIn);
				CloseHandle(hFileOut);
				return;
			}
			ReadFile(hFileIn,&glsd,sizeof(GIF_LOGICAL_SCREEN_DESCRIPTOR),&dw,0);
			if (glsd.iGctFlag) ReadFile(hFileIn,szColorTable,iGctSize[glsd.iSizeOfGct],&dw,0);
			if (glsd1.iWidth<glsd.iWidth) glsd1.iWidth=glsd.iWidth;
			if (glsd1.iHeight<glsd.iHeight) glsd1.iHeight=glsd.iHeight;
			for (;;) {
				ReadFile(hFileIn,&c,1,&dw,0);
				if (dw==0) {
					//MessageBox(hMainDlg,"Premature end of file encountered; no GIF image data present.",0,MB_ICONEXCLAMATION);
					printf("Premature end of file encountered; no GIF image data present.");
					CloseHandle(hFileIn);
					CloseHandle(hFileOut);
					return;
				}
				if (c==0x2C) {
					ReadFile(hFileIn,&gid,sizeof(GIF_IMAGE_DESCRIPTOR),&dw,0);
					if (gid.iLctFlag) {
						ReadFile(hFileIn,szColorTable,iGctSize[gid.iSizeOfLct],&dw,0);
					} else {
						gid.iLctFlag=1;
						gid.iSizeOfLct=glsd.iSizeOfGct;
					}
					break;
				} else if (c==0x21) {
					ReadFile(hFileIn,&c,1,&dw,0);
					if (c==0xF9) {
						ReadFile(hFileIn,&ggce,sizeof(GIF_GRAPHIC_CONTROL_EXTENSION),&dw,0);
					} else {
						for (;;) {
							ReadFile(hFileIn,&c,1,&dw,0);
							if (!c) break;
							SetFilePointer(hFileIn,c,0,FILE_CURRENT);
						}
					}
				}
			}
			ggce.iBlockSize=4;
			ggce.iDelayTime=delaytime;//delaytime 時間單位:cs(centi-seconds)
			ggce.iDisposalMethod=2;
			c=(char)0x21;
			WriteFile(hFileOut,&c,1,&dw,0);
			c=(char)0xF9;
			WriteFile(hFileOut,&c,1,&dw,0);
			WriteFile(hFileOut,&ggce,sizeof(GIF_GRAPHIC_CONTROL_EXTENSION),&dw,0);
			c=(char)0x2C;
			WriteFile(hFileOut,&c,1,&dw,0);
			WriteFile(hFileOut,&gid,sizeof(GIF_IMAGE_DESCRIPTOR),&dw,0);
			WriteFile(hFileOut,szColorTable,iGctSize[gid.iSizeOfLct],&dw,0);
			ReadFile(hFileIn,&c,1,&dw,0);
			WriteFile(hFileOut,&c,1,&dw,0);
			for (;;) {
				ReadFile(hFileIn,&c,1,&dw,0);
				WriteFile(hFileOut,&c,1,&dw,0);
				if (!c) break;
				ReadFile(hFileIn,szColorTable,c,&dw,0);
				WriteFile(hFileOut,szColorTable,c,&dw,0);
			}
			index++;
			CloseHandle(hFileIn);			
		}
		c=(char)0x3B;
		WriteFile(hFileOut,&c,1,&dw,0);
		SetFilePointer(hFileOut,6,0,FILE_BEGIN);
		WriteFile(hFileOut,&glsd1,sizeof(GIF_LOGICAL_SCREEN_DESCRIPTOR),&dw,0);
		CloseHandle(hFileOut);
	}
}
