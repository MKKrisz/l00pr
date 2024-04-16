#ifndef L00PR_AUDIO_DEVICE
#define L00PR_AUDIO_DEVICE

#include <SDL2/SDL.h>
#include "tune/tune.hpp"

class AudioDevice {
    /// The spec this thing should be using
    SDL_AudioSpec spec;
    SDL_AudioDeviceID devHandle = 0;

    /// The music this thing is playing
    //TODO: Add support for multiple tunes simultaenously
    std::vector<Tune> tunes;

    //TODO: make this work
    bool running;

    /// sets the spec. May or may not just delete the current device and create
    /// a new device instead
    void setSpec(SDL_AudioSpec& spec);
public:

    /// Sets the sample rate to the desired one
    void setSampleRate(int sr);
    /// Returns the sample rate
    int getSampleRate();


    void addTune(const Tune& t);
//    Tune getTune();

    AudioDevice(int srate = 48000, int bufsize = 4096);
    AudioDevice(SDL_AudioSpec& spec);
    AudioDevice(AudioDevice& dev);

    ~AudioDevice();

    ///callback. userdata is always a pointer to this
    static void callback(void* userdata, uint8_t* stream, int len);
    void start();
    void stop();

    AudioDevice& operator=(const AudioDevice& dev);
};

#endif


