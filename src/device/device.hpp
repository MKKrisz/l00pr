#pragma once

#include "../tune/tune.hpp"

class Device : public StringConvertible {
protected:
    bool running = false;
    bool cursed = false;
    std::vector<Tune*> tunes {};
    
protected:
    [[nodiscard]] virtual float getSample(int sampleRate, bool print) {
        float sum = 0;
        for(auto t : tunes) {
            sum += t->getSample(sampleRate, print);
        }
        return sum;
    }
    
public:
    virtual void start()     { running = true;  }
    virtual void stop()      { running = false; }
    virtual bool isRunning() { return running;  }

    virtual void setCursed(bool value) { cursed = value;}
    virtual bool isCursed() { return cursed; }
    
    virtual void setSampleRate(int value) = 0;
    virtual int  getSampleRate() = 0;

    virtual void fastForward(double t) {
        int srate = getSampleRate();
        for(int i = 0; i < t * srate; i++) {
            float _ = getSample(srate, false);
        }
    }

    void addTune(Tune& tune) { tunes.emplace_back(&tune); }

    virtual ~Device() {}
    std::string ToString() override { return "Device base"; }
};
