#pragma once

#include "device.hpp"

class RenderDevice : public Device {
public:
    void setSampleRate(int sr) override { srate = sr;   }
    int  getSampleRate() override       { return srate; }

    RenderDevice(std::string filename, int srate = 48000) : file(filename), srate(srate) {}

    void start() override;
    void stop()  override;
private:
    std::string file;
    int srate;
};
