#pragma once
#include "device.hpp"

class PrinterDevice : public Device {
public:
    void setSampleRate(int sr) override { srate = sr;   }
    int  getSampleRate() override       { return srate; }

    PrinterDevice(int srate = 48000) : srate(srate) {}

    void start() override;
    void stop()  override;
private:
    int srate;
};
