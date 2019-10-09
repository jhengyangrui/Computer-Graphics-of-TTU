///////////////////////////////////////////////////////////
// for BGmp3
////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "fmod.h"
#include "fmod_errors.h"

//
// samples are supposed for short effects (PCM data)
// to be loaded and played
//
// However, you may use your favorite mp3 as background music
// (if your platform supports it)
//
// there are two ways to do it:
// 1. the regular sound sample
// 2. the stream way
//
//

#define SAMP3  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/bgmp3/sixwords.mp3" 
#define SAMP4  "D:/jmchen09/game-lib/fmodapi374win/opengl_tests/bgmp3/KennyG.mp3" 

FSOUND_SAMPLE *samp3, *samp4;

FSOUND_STREAM *stream;
FSOUND_SAMPLE *sptr;


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



	samp4 = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMP4, FSOUND_NORMAL, 0,0);
	FSOUND_Sample_SetMode (samp4, FSOUND_LOOP_OFF);


    // mp3, ogg, wav: all works
	// only wav can store cue point (for sync)
    FSOUND_Stream_SetBufferSize(1000);
	stream = FSOUND_Stream_Open(SAMP3, FSOUND_NORMAL | FSOUND_MPEGACCURATE, 0, 0);
    if (!stream)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();

        return;
    }

    sptr = FSOUND_Stream_GetSample(stream);
}

static int channel3, channel4;

//
// simple play
//
void play ()
{
	// find a"free" channel to play the sample
	channel4 = FSOUND_PlaySound (FSOUND_FREE, samp4);
	printf ("channel: %d\n", channel4);
}

void playstream()
{
    // ==========================================================================================
    // PLAY STREAM
    // ==========================================================================================
    channel3 = FSOUND_Stream_PlayEx(FSOUND_FREE, stream, NULL, TRUE);
    FSOUND_SetPaused(channel3, FALSE);
}

//
// must obtain the right channel number to mute it
//
void togglesong ()
{
	static int toggle = 0;
	if (toggle) {
		// play 3
		FSOUND_SetPaused(channel4, TRUE);
		FSOUND_SetPaused(channel3, FALSE);
	} else {
		FSOUND_SetPaused(channel3, TRUE);
		FSOUND_SetPaused(channel4, FALSE);
	}
	toggle ^= 1;
}

void shutdown_fmod (void)
{
	FSOUND_Sample_Free (samp3);
	FSOUND_Sample_Free (samp4);
    FSOUND_Close();
}
