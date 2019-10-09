#include <windows.h>
#include <stdio.h>

#include "fmod.h"
#include "fmod_errors.h"

//
// samples are supposed for short effects (PCM data)
// to be loaded and played
//

#define SAMP3  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/gunshot/GE_Klobb.wav"     // 16000Hz, mono, 8-bit
#define SAMP4  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/gunshot/big_gun.wav"     // 16000Hz, mono, 8-bit
#define SAMP5  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/gunshot/lasrhvy1.wav"     // 16000Hz, mono, 8-bit

FSOUND_SAMPLE *samp3, *samp4, *samp5;

static	int channel3, channel4, channel5;
static int freq = 16000;

void init_fmod (void)
{
     
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        exit(1);
    }

    if (!FSOUND_Init(44100, 32, 0)) {
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        exit(1);
    }

	// preload samples
	samp3 = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMP3, FSOUND_NORMAL, 0,0);
	FSOUND_Sample_SetMode (samp3, FSOUND_LOOP_OFF);
	samp4 = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMP4, FSOUND_NORMAL, 0,0);
	FSOUND_Sample_SetMode (samp4, FSOUND_LOOP_OFF);
	samp5 = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMP5, FSOUND_NORMAL, 0,0);
	FSOUND_Sample_SetMode (samp5, FSOUND_LOOP_OFF);
	
	channel3 = FSOUND_PlaySoundEx(FSOUND_FREE, samp3, NULL, TRUE);
	channel4 = FSOUND_PlaySoundEx(FSOUND_FREE, samp4, NULL, TRUE);
	channel5 = FSOUND_PlaySoundEx(FSOUND_FREE, samp5, NULL, TRUE);
}


void playex (int which)
{
	FSOUND_SAMPLE *samp;
	int channel;

	switch (which) {
		case 1:
		channel = channel3; samp = samp3;
		break;
		case 2:
		channel = channel4; samp = samp4;
		break;
		case 3:
		channel = channel5; samp = samp5;
		break;
	}
			
	FSOUND_PlaySoundEx (channel, samp, NULL, TRUE);
	FSOUND_SetVolume(channel, 255);
    FSOUND_SetPan(channel, 128); 
	FSOUND_SetPaused (channel, FALSE);
}

void shutdown_fmod (void)
{
	FSOUND_Sample_Free (samp3);
	FSOUND_Sample_Free (samp4);
	FSOUND_Sample_Free (samp5);
    FSOUND_Close();
}
