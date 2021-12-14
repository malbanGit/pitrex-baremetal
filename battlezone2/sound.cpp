#include "game.h"
#include "sound.h"


#define NUM_TRACKS  200

sound::TRACK* sound::mTracks;


sound::sound()
{
    printf((char *)"Initing sound\n");

//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

    sound::mTracks = new TRACK[NUM_TRACKS];

    for (int i=0; i < NUM_TRACKS; i++)
    {
        mTracks[i].data = NULL;
        mTracks[i].playing = FALSE;
        mTracks[i].speed = 1;
    }
}

sound::~sound()
{
    stopSound();

    for (int i=0; i < NUM_TRACKS; i++)
    {
        if (mTracks[i].data)
            delete mTracks[i].data;
    }

    delete sound::mTracks;

//    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

}

void sound::bufferCallback(void *unused, Uint8 *stream, int len)
{
    Sint16 *buf = (Sint16 *)stream;

    // Clear the buffer
    memset(buf, 0, len);

    len /= 2;

    int i, s;

	const int max_audioval = ((1<<(16-1))-1);
	const int min_audioval = -(1<<(16-1));

    for (i=0; i<NUM_TRACKS; ++i)
    {
        TRACK* track = &mTracks[i];

        if (track->playing)
        {
        	Sint16 *data = (Sint16 *)track->data;

		    for (s=0; s < len; s++)
		    {
                {
                    float fPos = track->pos;

                    float iPos1 = (int)fPos;
                    float iPos2 = iPos1+1;

			        if (iPos2 >= track->len)
			        {
                        iPos2 -= track->len;
                    }

                    float interp = fPos - iPos1;

                    float v1 = (data[(int)iPos1] * (track->vol/4));
                    float v2 = (data[(int)iPos2] * (track->vol/4));

			        buf[s] += v1 + interp*(v2 - v1);

                    if (buf[s] > max_audioval) buf[s] = max_audioval;
                    else if (buf[s] < min_audioval) buf[s] = min_audioval;
                }
                {
                    float fPos = track->pos+1;

                    float iPos1 = (int)fPos;
                    float iPos2 = iPos1+1;

			        if (iPos2 >= track->len)
			        {
                        iPos2 -= track->len;
                    }

                    float interp = fPos - iPos1;

                    float v1 = (data[(int)iPos1] * (track->vol/4));
                    float v2 = (data[(int)iPos2] * (track->vol/4));

			        buf[s+1] += v1 + interp*(v2 - v1);

                    if (buf[s+1] > max_audioval) buf[s+1] = max_audioval;
                    else if (buf[s+1] < min_audioval) buf[s+1] = min_audioval;
                }

                track->pos += track->speed;

/*
    {
        // Advance pointer in buffer
        fine_pos += speed;
        int_pos   = (int) fine_pos;
        frac_pos  = fine_pos - int_pos;

        // Do not exceed buffer length!
        while( fine_pos > my_sample_len ) fine_pos -= my_sample_len;

        // Perform a linear interpolation
        signed char x1 = my_sample[ int_pos ];
        signed char x2 = my_sample[ ( int_pos == my_sample_len-1 ) ? 0 : int_pos+1 ];

        // Write to buffer...
        snd_buffer[i] = (signed char) ( x1 + frac_pos*(x2 - x1) );
    }

*/

			    if (track->pos >= track->len)
			    {
                    if (track->loop)
                    {
                        track->pos -= track->len;
                    }
                    else
                    {
                        track->pos = 0;
                        track->playing = FALSE;
                        break;
                    }
			    }
		    }
        }
    }

}

void sound::loadTrack(char *file, int track, float volume, bool loop/*=false*/)
{
  // TODO
/*  
    int index;
    SDL_AudioSpec wave;
    SDL_AudioSpec desired;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    // Load the sound file and convert it to 16-bit stereo at 44Hz
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL )
    {
//        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());

        TCHAR s[512];
        wsprintf(s, L"Failed loading audio track %d\n", track);
        OutputDebugString(s);

        return;
    }

    TCHAR s[512];
    wsprintf(s, L"Loaded audio track %d\n", track);
    OutputDebugString(s);

    desired = wave;
    desired.channels = 2;
    desired.freq = 44100;
    desired.format = AUDIO_S16;
    desired.callback = sound::bufferCallback;

    SDL_BuildAudioCVT(&cvt,
        wave.format,
        wave.channels,
        wave.freq,
        AUDIO_S16,
        2,
        44100);

    cvt.buf = (Uint8*)malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_FreeWAV(data);

    if (SDL_ConvertAudio(&cvt) != 0)
    {
        TCHAR s[256];
        wsprintf(s, L"Failed to convert track %d\n", track);
        OutputDebugString(s);
    }

    // Put the sound data in the slot (it starts playing immediately) 
    if (mTracks[track].data)
    {
        delete mTracks[track].data;
        mTracks[track].data = NULL;
    }
    SDL_LockAudio();
    mTracks[track].data = cvt.buf;
    mTracks[track].len = cvt.len_cvt/2;
    mTracks[track].pos = 0;
    mTracks[track].loop = loop;
    mTracks[track].vol = volume;
    mTracks[track].playing = FALSE;
    SDL_UnlockAudio();

    if ( SDL_OpenAudio(&desired, NULL) < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    }
    */
}

void sound::startSound()
{
    //SDL_PauseAudio(0);
}

void sound::stopSound()
{
    //SDL_PauseAudio(1);
}

void sound::playTrack(int track)
{
//    SDL_LockAudio();
    mTracks[track].pos = 0;
    mTracks[track].playing = TRUE;
//    SDL_UnlockAudio();
}

void sound::stopTrack(int track)
{
//    SDL_LockAudio();
    mTracks[track].pos = 0;
    mTracks[track].playing = FALSE;
//    SDL_UnlockAudio();
}

void sound::stopAllTracks()
{
//    SDL_LockAudio();
    for (int i=0; i<NUM_TRACKS; i++)
    {
        mTracks[i].pos = 0;
        mTracks[i].playing = FALSE;
    }
//    SDL_UnlockAudio();
}

void sound::setTrackSpeed(int track, double speed)
{
    mTracks[track].speed = speed;
}

bool sound::isTrackPlaying(int track)
{
    bool playing;

//    SDL_LockAudio();
    playing = mTracks[track].playing;
//    SDL_UnlockAudio();

    return playing;
}

int sound::playTrackGroup(int trackFirst, int trackLast)
{
    for (int i=trackFirst; i<=trackLast; i++)
    {
        if (!isTrackPlaying(i))
        {
            playTrack(i);
            return i;
        }
    }
    // Just retrigger the first one
    playTrack(trackFirst);
    return trackFirst;
}
