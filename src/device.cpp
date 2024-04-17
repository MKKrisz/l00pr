
#include "device.h"

#include <iostream> // for debug
#include <cstring>  // for memcpy

void AudioDevice::setSampleRate(int sr) {
    SDL_CloseAudioDevice(this->devHandle);
    *this = AudioDevice(sr, spec.samples);
}

int AudioDevice::getSampleRate() { return spec.freq; }

void AudioDevice::addTune(const Tune& t) {
    tunes.emplace_back(t);
}

//Tune* AudioDevice::getTune() { return tune; }

void AudioDevice::setSpec(SDL_AudioSpec& s) {
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

AudioDevice::AudioDevice(int srate, int bufsize) : tunes() {
    spec.freq = srate;
    spec.format = AUDIO_F32;
    spec.channels = 1;
    spec.samples = bufsize;
    spec.callback =  &(this->callback);
    spec.userdata = this;
    setSpec(spec);
}

AudioDevice::AudioDevice(SDL_AudioSpec& spec) : tunes() {
    setSpec(spec);    
}

AudioDevice::AudioDevice(AudioDevice& dev) : tunes() {
    *this = dev;
}

AudioDevice::~AudioDevice() {
    SDL_CloseAudioDevice(devHandle);
}

void AudioDevice::fastForward(double t) {
    SDL_LockAudioDevice(devHandle);
    int srate = getSampleRate();
    for(int i = 0; i < t * srate; i++)
        tunes[0].discardSample(srate);
    SDL_UnlockAudioDevice(devHandle);
}

void AudioDevice::callback(void* userdata, uint8_t* stream, int bytelen) {
    int len = bytelen / 4;
    AudioDevice* dev = (AudioDevice*)userdata;
    for(int i = 0; i < len; i++) {
        float sample = dev->tunes[0].getSample(dev->getSampleRate());
        //std::cout << sample << std::endl;
        std::memcpy(&stream[4*i], &sample, 4);
    }
}

void AudioDevice::start() {
    SDL_PauseAudioDevice(devHandle, 0);
}
void AudioDevice::stop() {
    SDL_PauseAudioDevice(devHandle, 1);
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
