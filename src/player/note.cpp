#include "note.hpp"
#include "../util.hpp"
#include <cmath>

Note::Note(double l, Interpolated<Frequency> freq, Interpolated<double> amp) 
    : len(l), freq(freq), ampl(amp), done(0) {
    freq.SetInterpolator(logarithmicInterpolator<Frequency>);
    amp.SetInterpolator(logarithmicInterpolator<double>);
}

bool Note::isComplete() {
    double tp = 1/freq(done/len).getFreq();
    return done > len + (tp-fmod(len, tp));
}

double Note::getDelta(int srate) {
    done += 1/double(srate);
    return freq(done/len).getFreq()/srate;
}
double Note::getAmplitude(double t) {
    return ampl(t);
}
double Note::getAmplitude() {
    //falloff: @^0.02
    return ampl(done/len);
}

double Note::getFreq(double t) {
    return freq(t).getFreq();
}
double Note::getFreq() {
    return freq(done/len).getFreq();
}

Note::Note(std::istream& stream, double bpm) : len(0), freq(), ampl(1), done(0){
    if(!isNote((stream >> skipws).peek()))
        throw parse_error(stream, "Cannot parse note");
    freq.Clear();
    stream >> freq;

    if(!isdigit((stream >> skipws).peek()))
        throw parse_error(stream, std::string("Unexpected ") + char(stream.peek()));
    stream >> len;
    len *= 60/bpm;

    if(isdigit((stream >> skipws).peek())) {
        ampl.Clear();
        stream >> ampl;
    }
}

std::ostream& operator<<(std::ostream& str, Note& n) {
    return str << n.toString(); 
}
