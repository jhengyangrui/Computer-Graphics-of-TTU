#include <windows.h>
#include <math.h>

#include "fmod.h"
#include "fmod_errors.h"
#include "svl/svl.h"

#include <stdio.h>

FSOUND_SAMPLE *samp;
int channel1;
#define SAMPLE_FILE "siren.wav"
//#define SAMPLE_FILE "ambulance.wav"

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

	samp = FSOUND_Sample_Load (FSOUND_UNMANAGED, SAMPLE_FILE, FSOUND_LOOP_NORMAL, 0,0);
    
	// increasing min_distance makes it louder in 3d space
    FSOUND_Sample_SetMinMaxDistance(samp, 4.0f, 10000.0f);
    FSOUND_Sample_SetMode(samp, FSOUND_LOOP_NORMAL);
	FSOUND_3D_SetDopplerFactor (8.0);

	// default 3D source set up
	{
        float pos[3] = { -8.0f, 0.0f, 5.0f };
        float vel[3] = { 0,0,0 };

        channel1 = FSOUND_PlaySoundEx(FSOUND_FREE, samp, NULL, TRUE);
		FSOUND_SetFrequency (channel1, 11025);
        FSOUND_3D_SetAttributes(channel1, pos, vel);
        if (!FSOUND_SetPaused(channel1, FALSE))
        {
            printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        }
    }


#if 1
	// default listener setup
	{
		float pos[3] = {0,0,0};

		FSOUND_3D_Listener_SetAttributes (pos, NULL, 0,0,1, 0,1,0);
	}
#endif

}

void Update (const Vec3 &rhpos, const Vec3 &rhvel)
{
	float pos[3], vel[3];

	pos[0] = rhpos[0];
	pos[1] = rhpos[1];
	pos[2] = -rhpos[2];   // LH coord sys
	
	vel[0] = rhvel[0];
	vel[1] = rhvel[1];
	vel[2] = -rhvel[2];

	FSOUND_3D_SetAttributes (channel1, pos, vel);

//	FSOUND_3D_GetAttributes (channel1, pos, vel);
//	printf ("[%.2f]: [%.3f %.3f %.3f]\n",angle,pos[0],pos[1],pos[2]);

	FSOUND_Update();
}


void shutdown_fmod (void)
{
    // FREE SONG AND SHUT DOWN
	FSOUND_Sample_Free (samp); 
    FSOUND_Close();
}
