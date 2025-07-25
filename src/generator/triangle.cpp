#include "triangle.hpp"
#include "../util.hpp"
#include "value.hpp"

TriangleGenerator::TriangleGenerator(Interpolated<double> amplitude, 
        Interpolated<double> phasemul, 
        Interpolated<double> offset, 
        Interpolated<double> peak) 
    : Generator(phasemul, amplitude, offset), m_peak(std::make_unique<ValueGenerator>(peak)){}

double TriangleGenerator::getSample(double p, double t, int srate) {
    p = fmod(p, 1);
    double fm = m_peak->getFrequencyMultiplier(t, srate);
    (*m_peak)(t*fm, t, srate, 1);
    double peak = m_peak->getSample(srate);
    if(p < peak/2)
        return lerp(0.0f, 1.0f, p*2/peak);
    else if(p < (1-(peak/2)))
        return lerp(1.0f, -1.0f, (p-(peak/2))/(1-peak));
    else
        return lerp(-1.0f, 0.0f, (p+(peak/2)-1)*2/peak);
}

TriangleGenerator::TriangleGenerator(const TriangleGenerator& g) 
    : Generator(g), m_peak(g.m_peak->copy()) {}

TriangleGenerator::TriangleGenerator(std::istream& stream, int srate) : Generator(stream, srate), m_peak(std::make_unique<ValueGenerator>(0.5)){
    if(shouldBeDefault) { 
        return;
    }
    if((stream >> skipws).peek() != ')') {
        m_peak = Source::Make(stream, srate, MakeFlags::all);
    }
    stream >> expect(')');
}

std::unique_ptr<Source> TriangleGenerator::copy() {
    return std::make_unique<TriangleGenerator>(*this);
}

std::unique_ptr<TriangleGenerator> TriangleGenerator::Create(std::istream& str, const int srate, const MakeFlags&) {
    return std::make_unique<TriangleGenerator>(str, srate);
}
