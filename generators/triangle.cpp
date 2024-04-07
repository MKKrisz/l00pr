#include "triangle.hpp"
#include "../util.hpp"

TriangleGenerator::TriangleGenerator(Interpolated<float> amplitude, Interpolated<float> phasemul, Interpolated<float> offset, Interpolated<float> peak) {
    m_gain = amplitude;
    m_phasemul = phasemul;
    m_phaseoffset = offset;
    m_peak = peak;
}

float TriangleGenerator::getSample(float p, float t) {
    float peak = m_peak.Get(t);
    if(p < peak/2)
        return lerp(0.0f, 1.0f, p*2/peak);
    else if(p < (1-(peak/2)))
        return lerp(1.0f, -1.0f, (p-(peak/2))/(1-peak));
    else
        return lerp(-1.0f, 0.0f, (p+(peak/2)-1)*2/peak);
}

TriangleGenerator::TriangleGenerator(std::istream& stream) : TriangleGenerator() {
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
    m_peak = a[3];
}

Generator* TriangleGenerator::copy() { 
    TriangleGenerator* ret = new TriangleGenerator(m_gain, m_phasemul, m_phaseoffset);
    ret->phases = phases;
    return ret;
}

