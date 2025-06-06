#ifndef L00PR_AUDIO_DEVICE
#define L00PR_AUDIO_DEVICE

#ifdef _WIN32
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "device.hpp"

/// <summary> Device to manage SDL audio and play your funky tunes. </summary>
class AudioDevice : public Device {
    /// <summary> The spec this thing should be using. </summary>
    SDL_AudioSpec spec;

    /// <summary> The device handle given by SDL. If set to 0, it is invalid. (see: https://wiki.libsdl.org/SDL2/SDL_OpenAudioDevice) </summary>
    SDL_AudioDeviceID devHandle = 0;

    /// <summary> Sets the specification to use </summary>
    void setSpec(const SDL_AudioSpec& spec);
public:

    /// <summary> Sets the sample rate to the desired one. </summary>
    void setSampleRate(int sr) override;
    /// <summary> Returns the current sample rate. </summary>
    int getSampleRate() override;

    // Constructors
    AudioDevice(int srate = 48000, int bufsize = 4096);
    AudioDevice(SDL_AudioSpec& spec);
    AudioDevice(const AudioDevice& dev);

    // Destructor
    ~AudioDevice() override;

    /// <summary> 
    /// Callback function. Gets called by SDL, not meant to be used by user. 
    /// (see: https://wiki.libsdl.org/SDL2/SDL_AudioSpec)
    /// </summary>
    /// <param name="userdata"> Pointer to this AudioDevice object. </param>
    /// <param name="stream"> Pointer to an array SDL wants to get filled. </param>
    /// <param name="len"> Length of the stream </param>
    static void callback(void* userdata, uint8_t* stream, int len);

    /// <summary> Starts playing </summary>
    void start() override;
    /// <summary> Stops playing </summary>
    void stop() override;

    // Copy assignment operator
    AudioDevice& operator=(const AudioDevice& dev);
};

#endif


