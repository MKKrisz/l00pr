#include "playablenote.hpp"
#include "../util.hpp"
#include <cmath>

PlayableNote::PlayableNote(double l, Interpolated<Frequency> freq, Interpolated<double> amp, double done) 
    : m_length(l), m_frequency(freq), m_amplitude(amp), done(done) {
    m_frequency.SetInterpolator(logarithmicInterpolator<Frequency>);
    amp.SetInterpolator(logarithmicInterpolator<double>);
}

bool PlayableNote::IsComplete() const {
    //double tp = 1/m_frequency(done/m_length).getFreq();
    return done > m_length; //- (tp-fmod(m_length, tp));
}

void PlayableNote::advance(int srate, double fm) {
    phase = fmod(phase + getDelta(srate) * fm, 1);
    done += 1/double(srate);
}

double PlayableNote::getDelta(int srate) {
    return m_frequency(done/m_length).getFreq()/srate;
}
double PlayableNote::getAmplitude(double t) {
    return m_amplitude(t);
}
double PlayableNote::getAmplitude() {
    return m_amplitude(done/m_length);
}

double PlayableNote::getFreq(double t) {
    return m_frequency(t).getFreq();
}
double PlayableNote::getFreq() {
    return m_frequency(done/m_length).getFreq();
}

PlayableNote::PlayableNote(std::istream& stream, double bpm) : Note(), m_length(0), m_frequency(), m_amplitude(1), done(0){
    bool starts_with_parenthesis = false;
    if((stream >> skipws).peek() == '(') {stream.get(); starts_with_parenthesis = true; }
    if(!isNote((stream >> skipws).peek()))
        throw parse_error(stream, "Cannot parse note");
    m_frequency.Clear();
    stream >> m_frequency;

    if(!isdigit((stream >> skipws).peek()))
        throw parse_error(stream, std::string("Unexpected ") + char(stream.peek()));
    stream >> m_length;
    m_length *= 60/bpm;

    if(isdigit((stream >> skipws).peek())) {
        m_amplitude.Clear();
        stream >> m_amplitude;
    }

    if(starts_with_parenthesis) {stream >> expect(')');}
}

std::unique_ptr<PlayableNote> PlayableNote::Create(std::istream& str, Tune*, double bpm, bool, int) {
    return std::make_unique<PlayableNote>(str, bpm);
}

void PlayableNote::Write(std::ostream& str) const {
    str << "note(";
    m_frequency.Write(str);
    str << "  " << m_length << "  ";
    m_amplitude.Write(str);
    str << ")";
}

std::ostream& operator<<(std::ostream& str, PlayableNote& n) {
    return str << n.ToString(); 
}
