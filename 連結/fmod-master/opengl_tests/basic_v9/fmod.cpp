//////////////////////////////////////////////////
// for basic fmod
//////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>

#include "fmod.h"
#include "fmod_errors.h"

//
// samples are supposed for short effects (PCM data)
// to be loaded and played
//

#define SAMP3  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/basic_v9/saxdemo.wav"     // 22050 Hz, mono, 16-bit

FSOUND_SAMPLE *samp3;
static int freq = 22050;

void init_fmod ()
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

//
// simple play
//
void play ()
{
	// find a"free" channel to play the sample
	channel = FSOUND_PlaySound (FSOUND_FREE, samp3);
	printf ("channel: %d\n", channel);
}


void playex ()
{
	channel = FSOUND_PlaySoundEx(FSOUND_FREE, samp3, NULL, TRUE);
	printf ("channel: %d\n", channel);
//    FSOUND_SetCurrentPosition(channel, FSOUND_Sample_GetLength(samp3)-1);
//   FSOUND_SetFrequency(channel, -22050);   /* Play it backwards! */
    FSOUND_SetFrequency (channel, freq);   // change playback freq
	FSOUND_SetVolume(channel, 255);
    FSOUND_SetPan(channel, 255);            /* pan it all the way to the right */

	// make it loop
	FSOUND_SetLoopMode (channel, FSOUND_LOOP_NORMAL); 
    FSOUND_SetPaused(channel, FALSE);
}

void freq_up()
{
	freq *= 1.059463;  // up one semitone
	FSOUND_SetFrequency (channel, freq);
}

void freq_down()
{
	freq /= 1.059463; // down one semitone
	FSOUND_SetFrequency (channel, freq);
}

//
// must obtain the right channel number to mute it
//

void toggle_pause()
{
	FSOUND_SetPaused (channel, ! FSOUND_GetPaused (channel));
}

void stopsound ()
{
	FSOUND_StopSound (channel);
}

void shutdown_fmod ()
{
	FSOUND_Sample_Free (samp3);
    FSOUND_Close();
}
