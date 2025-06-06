#include "renderdevice.hpp"
#include <fstream>

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

void RenderDevice::start() {
    Device::start();

    std::ofstream stream {file};
    if(!stream.good()) {
        throw std::runtime_error("Could not open file: " + file + " for writing!");
    }

    stream << "RIFF";
    int size = stream.tellp();          //Position of "size" for later
    stream << "    WAVEfmt ";
    put32(stream, 16);                  //length of fmt chunk
    if(cursed)
        put16(stream, 1);                   //PCM int
    else
        put16(stream, 3);                   //PCM float
    put16(stream, 1);                   //Mono
    put32(stream, srate);           //Sample rate
    put32(stream, srate * 4);       //Byte rate
    put16(stream, 2);                   //Block size
    put16(stream, 32);                  //Bits
    stream << "data";
    int size2 = stream.tellp();         //Position of chunk size for later
    stream << "    ";
    uint samples = 0;
    while(!tunes[0]->isComplete()) {
        if(cursed)
            put32(stream, (getSample(srate, true) + 1) / 2 * std::numeric_limits<uint>::max());
        else
#ifdef _WIN32
            put32(stream, bit_cast<uint>(float(getSample(srate, true))));
#else
            put32(stream, std::bit_cast<uint>(float(getSample(srate, true))));
#endif
        samples++;
    }
    stream.flush();
    stream.seekp(size);
    put32(stream, samples * 4 + 36);
    stream.seekp(size2);
    put32(stream, samples * 4);

    stream.close();

    Device::stop();
}
void RenderDevice::stop() {}
