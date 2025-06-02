
#include "audiodevice.hpp"

#include <iostream> // for debug
#include <cstring>  // for memcpy

#ifdef _WIN32
typedef unsigned int uint;
typedef unsigned short ushort;

//For some reason msvc does not know what bit_cast is
template<class To, class From>
std::enable_if_t<
    sizeof(To) == sizeof(From) &&
    std::is_trivially_copyable_v<From>&&
    std::is_trivially_copyable_v<To>,
    To>
    // constexpr support needs compiler magic
    bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>,
        "This implementation additionally requires "
        "destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

#endif

void AudioDevice::setSampleRate(int sr) {
    SDL_CloseAudioDevice(this->devHandle);
    spec.freq = sr;
    setSpec(spec);
}

int AudioDevice::getSampleRate() { return spec.freq; }

//Tune* AudioDevice::getTune() { return tune; }

void AudioDevice::setSpec(const SDL_AudioSpec& s) {
    std::cout << "Asked for: " <<  s.freq << " " << s.format << " " << s.channels << " " << s.samples << std::endl;;
    SDL_AudioSpec temp;
    devHandle = SDL_OpenAudioDevice(nullptr, 0, &s, &temp, 0);
    //std::cout << "dev: " << devHandle << std::endl;
    //spec = temp;
    spec = s;
    std::cout << "Got      : " <<  spec.freq << " " << spec.format << " " << spec.channels << " " << spec.samples << std::endl;;
    if(devHandle == 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}

AudioDevice::AudioDevice(int srate, int bufsize) {
    spec.freq = srate;
    spec.format = AUDIO_F32;
    spec.channels = 1;
    spec.samples = bufsize;
    spec.callback =  &(this->callback);
    spec.userdata = this;
    setSpec(spec);
}

AudioDevice::AudioDevice(SDL_AudioSpec& spec) {
    setSpec(spec);    
}

AudioDevice::AudioDevice(const AudioDevice& dev) {
    *this = dev;
}

AudioDevice::~AudioDevice() {
    SDL_CloseAudioDevice(devHandle);
}

void AudioDevice::callback(void* userdata, uint8_t* stream, int bytelen) {
    int len = bytelen / 4;
    AudioDevice* dev = (AudioDevice*)userdata;
    for(int i = 0; i < len; i++) {
        float sample = dev->getSample(dev->getSampleRate(), true);
        if(dev->cursed) {sample += (sample < 0 ? 1 : -1);}
        //std::cout << sample << std::endl;
        std::memcpy(&stream[4*i], &sample, 4);
    }
}

void AudioDevice::start() {
    SDL_PauseAudioDevice(devHandle, 0);
    Device::start();
}
void AudioDevice::stop() {
    SDL_PauseAudioDevice(devHandle, 1);
    Device::stop();
}


AudioDevice& AudioDevice::operator=(const AudioDevice& dev) {
    if(this == &dev) {
        return *this;
    }
    spec = dev.spec;
    devHandle = dev.devHandle;
    tunes = dev.tunes;
    running = dev.running;
    return *this;
}
