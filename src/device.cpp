
#include "device.h"

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
    *this = AudioDevice(sr, spec.samples);
}

int AudioDevice::getSampleRate() { return spec.freq; }

void AudioDevice::addTune(Tune& t) {
    tunes.emplace_back(&t);
}

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

AudioDevice::AudioDevice(const AudioDevice& dev) : tunes() {
    *this = dev;
}

AudioDevice::~AudioDevice() {
    SDL_CloseAudioDevice(devHandle);
}

void AudioDevice::fastForward(double t) {
    SDL_LockAudioDevice(devHandle);
    int srate = getSampleRate();
    for(int i = 0; i < t * srate; i++)
        tunes[0]->getSample(srate, false);
    SDL_UnlockAudioDevice(devHandle);
}

void AudioDevice::callback(void* userdata, uint8_t* stream, int bytelen) {
    int len = bytelen / 4;
    AudioDevice* dev = (AudioDevice*)userdata;
    for(int i = 0; i < len; i++) {
        float sample = dev->tunes[0]->getSample(dev->getSampleRate(), true);
        if(dev->cursed) {sample += (sample < 0 ? 1 : -1);}
        //std::cout << sample << std::endl;
        std::memcpy(&stream[4*i], &sample, 4);
    }
}

void AudioDevice::start(bool cursed) {
    this->cursed = cursed;
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

void put32(std::ostream& str, uint n) {
    // TODO mi van ha a számítógéped olyan mint a wav
    str.put(n << 24 >> 24);
    str.put(n >> 8 << 24 >> 24);
    str.put(n >> 16 << 24 >> 24);
    str.put(n >> 24);
}
void put16(std::ostream& str, ushort n) {
    str.put(n << 8 >> 8);
    str.put(n >> 8);
}

void AudioDevice::render(std::ostream& stream, bool cursed) {
    stream << "RIFF";
    int size = stream.tellp();          //Position of "size" for later
    stream << "    WAVEfmt ";
    put32(stream, 16);                  //length of fmt chunk
    if(cursed)
        put16(stream, 1);                   //PCM int
    else
        put16(stream, 3);                   //PCM float
    put16(stream, 1);                   //Mono
    put32(stream, spec.freq);           //Sample rate
    put32(stream, spec.freq * 4);       //Byte rate
    put16(stream, 2);                   //Block size
    put16(stream, 32);                  //Bits
    stream << "data";
    int size2 = stream.tellp();         //Position of chunk size for later
    stream << "    ";
    uint samples = 0;
    while(!tunes[0]->isComplete()) {
        if(cursed)
            put32(stream, (tunes[0]->getSample(spec.freq) + 1) / 2 * std::numeric_limits<uint>::max());
        else
#ifdef _WIN32
            put32(stream, bit_cast<uint>(float(tunes[0].getSample(spec.freq))));
#else
            put32(stream, std::bit_cast<uint>(float(tunes[0]->getSample(spec.freq))));
#endif
        samples++;
    }
    stream.flush();
    stream.seekp(size);
    put32(stream, samples * 4 + 36);
    stream.seekp(size2);
    put32(stream, samples * 4);
    stream.flush();
}

