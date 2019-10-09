///////////////////////////////////////////////////////////////
// for engine sound
///////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "fmod.h"
#include "fmod_errors.h"

//
// samples are supposed for short effects (PCM data)
// to be loaded and played
//

#define SAMP3  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/engine/running.wav"     

FSOUND_SAMPLE *samp3;
static int freq = 10000;

void init_fmod (void)
{
     
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        exit(1);
    }

    // INITIALIZE
    if (!FSOUND_Init(44100, 32, 0)) {
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        exit(1);
    }

	samp3 = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMP3, FSOUND_NORMAL, 0,0);
	FSOUND_Sample_SetMode (samp3, FSOUND_LOOP_OFF);//NORMAL);
}

static	int channel;

void playex ()
{
	channel = FSOUND_PlaySoundEx(FSOUND_FREE, samp3, NULL, TRUE);
	printf ("channel: %d\n", channel);
    FSOUND_SetFrequency (channel, freq);   // change playback freq
	FSOUND_SetVolume(channel, 255);
    FSOUND_SetPan(channel, 255);            /* pan it all the way to the right */
	FSOUND_SetLoopMode (channel, FSOUND_LOOP_NORMAL);   // make it loop
    FSOUND_SetPaused(channel, FALSE);
}

void play_freq (double freq_ratio)
{
	FSOUND_SetFrequency (channel, freq_ratio*freq);
}


//
// must obtain the right channel number to mute it
//
void stopsound (void)
{
	FSOUND_StopSound (channel);
}

void shutdown_fmod (void)
{
	FSOUND_Sample_Free (samp3);
    FSOUND_Close();
}
