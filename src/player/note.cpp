#include "note.hpp"
#include "../util.hpp"
#include <cmath>

Note::Note(float l, Interpolated<double> freq, Interpolated<double> amp) 
    : len(l), freq(freq), ampl(amp), done(0) {
    freq.SetInterpolator(logarithmicInterpolator<float>);
    amp.SetInterpolator(logarithmicInterpolator<float>);
}

bool Note::isComplete() {
    float tp = 1/freq(done/len);
    return done > len + (tp-fmod(len, tp));
}

float Note::getDelta(int srate) {
    done += 1/float(srate);
    return freq(done/len)/srate;
}
float Note::getAmplitude(float t) {
    return ampl(t);
}
float Note::getAmplitude() {
    //falloff: @^0.02
    return ampl(done/len);
}

float Note::getFreq(float t) {
    return freq(t);
}
float Note::getFreq() {
    return freq(done/len);
}

Note::Note(std::istream& stream, double bpm) : len(0), freq(), ampl(1), done(0){
    if(!isNote((stream >> std::ws).peek()))
        throw parse_error(stream, "Cannot parse note");
    freq.Clear();
    stream >> freq;

    if(!isdigit((stream >> std::ws).peek()))
        throw parse_error(stream, std::string("Unexpected ") + char(stream.peek()));
    stream >> len;
    len *= 60/bpm;

    if(isdigit((stream >> std::ws).peek())) {
        ampl.Clear();
        stream >> ampl;
    }
}

