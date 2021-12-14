#ifndef SOUND_H_
#define SOUND_H_

//#include "SDL.h"


class sound
{
public:
    sound();
    ~sound();

    void loadTrack(char *file, int track, float volume, bool loop=false);
    void playTrack(int track);
    void stopTrack(int track);
    void stopAllTracks();
    void setTrackSpeed(int track, double speed);
    int playTrackGroup(int trackFirst, int trackLast);
    bool isTrackPlaying(int track);

    void startSound();
    void stopSound();

private:

    static void bufferCallback(void *unused, Uint8 *stream, int len);

    typedef struct {
        Uint8 *data;
        bool loop;
        bool playing;
        float vol;
        double speed;
        double pos;
        Uint32 len;
    } TRACK;

    static TRACK* mTracks;

};


#endif
