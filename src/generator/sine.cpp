#include "sine.hpp"

#include <cmath>

SineGenerator::SineGenerator(Interpolated<float> amplitude,
        Interpolated<float> phasemul, 
        Interpolated<float> offset) {
    m_gain = amplitude;
    m_phasemul = phasemul;
    m_phaseoffset = offset;
}

float SineGenerator::getSample(float p, float t) {
    return std::sin(p*2*M_PI);
}

SineGenerator::SineGenerator(std::istream& stream) : SineGenerator() {
    if((stream >> std::ws).peek() != '(') return;
    stream.get();
    Interpolated<float> a[3] = {1, 1, 0.0f};
    for(int i = 0; i < 3; i ++) {
        if((stream >> std::ws).peek() == ')') break;
        a[i].Clear();
        stream >> a[i];
    }
    if((stream >> std::ws).peek() != ')') 
        throw parse_error(stream, "Excepted ')'");
    stream.get();
    m_gain = a[1];
    m_phasemul = a[0];
    m_phaseoffset = a[2];
}

Generator* SineGenerator::copy() { 
    SineGenerator* ret = new SineGenerator(m_gain, m_phasemul, m_phaseoffset);
    ret->phases = phases;
    return ret;
}

