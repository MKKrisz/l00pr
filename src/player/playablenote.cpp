#include "playablenote.hpp"
#include "../util.hpp"
#include <cmath>

PlayableNote::PlayableNote(double l, Interpolated<Frequency> freq, Interpolated<double> amp, double done) 
    : len(l), freq(freq), ampl(amp), done(done) {
    freq.SetInterpolator(logarithmicInterpolator<Frequency>);
    amp.SetInterpolator(logarithmicInterpolator<double>);
}

bool PlayableNote::IsComplete() const {
    //double tp = 1/freq(done/len).getFreq();
    return done > len; //- (tp-fmod(len, tp));
}

double PlayableNote::getDelta(int srate) {
    done += 1/double(srate);
    return freq(done/len).getFreq()/srate;
}
double PlayableNote::getAmplitude(double t) {
    return ampl(t);
}
double PlayableNote::getAmplitude() {
    return ampl(done/len);
}

double PlayableNote::getFreq(double t) {
    return freq(t).getFreq();
}
double PlayableNote::getFreq() {
    return freq(done/len).getFreq();
}

PlayableNote::PlayableNote(std::istream& stream, double bpm) : len(0), freq(), ampl(1), done(0){
    bool starts_with_parenthesis = false;
    if((stream >> skipws).peek() == '(') {stream.get(); starts_with_parenthesis = true; }
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

    if(starts_with_parenthesis) {stream >> expect(')');}
}
PlayableNote* PlayableNote::Create(std::istream& str, const std::vector<AudioSource*>&, double bpm, bool, int) {
    return new PlayableNote(str, bpm);
}
void PlayableNote::Write(std::ostream& str) const {
    str << "note(";
    freq.Write(str);
    str << "  " << len << "  ";
    ampl.Write(str);
    str << ")";
}

std::ostream& operator<<(std::ostream& str, PlayableNote& n) {
    return str << n.ToString(); 
}
