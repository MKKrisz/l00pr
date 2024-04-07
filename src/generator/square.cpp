#include "square.hpp"

SquareGenerator::SquareGenerator(Interpolated<float> amplitude, Interpolated<float> phasemul, Interpolated<float> offset, Interpolated<float> duty) {
    m_gain = amplitude;
    m_phasemul = phasemul;
    m_phaseoffset = offset;
    m_dutyCycle = duty;
}

float SquareGenerator::getSample(float p, float t) {
    return p>m_dutyCycle(t)? 1 : -1;
}


SquareGenerator::SquareGenerator(std::istream& stream) : SquareGenerator() {
    if((stream >> std::ws).peek() != '(') return;
    stream.get();
    Interpolated<float> a[4] = {1, 1, 0.0f, 0.5f};
    for(int i = 0; i < 4; i ++) {
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
    m_dutyCycle = a[3];
}

Generator* SquareGenerator::copy() { 
    SquareGenerator* ret = new SquareGenerator(m_gain, m_phasemul, m_phaseoffset, m_dutyCycle);
    ret->phases = phases;
    return ret;
}
