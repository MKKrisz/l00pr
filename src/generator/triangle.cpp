#include "triangle.hpp"
#include "../util.hpp"

TriangleGenerator::TriangleGenerator(Interpolated<double> amplitude, 
        Interpolated<double> phasemul, 
        Interpolated<double> offset, 
        Interpolated<double> peak) 
    : Generator(phasemul, amplitude, offset), m_peak(peak){}

double TriangleGenerator::getSample(double p, double t) {
    double peak = m_peak.Get(t);
    if(p < peak/2)
        return lerp(0.0f, 1.0f, p*2/peak);
    else if(p < (1-(peak/2)))
        return lerp(1.0f, -1.0f, (p-(peak/2))/(1-peak));
    else
        return lerp(-1.0f, 0.0f, (p+(peak/2)-1)*2/peak);
}

TriangleGenerator::TriangleGenerator(const TriangleGenerator& g) 
    : Generator(g), m_peak(g.m_peak) {}

TriangleGenerator::TriangleGenerator(std::istream& stream) : Generator(stream), m_peak(0.5){
    if(shouldBeDefault) { 
        parse_lb(stream);
        return;
    }
    if((stream >> skipws).peek() != ')') {
        m_peak.Clear();
        stream >> m_peak;
    }
    stream >> expect(')');
    parse_lb(stream);
}

std::unique_ptr<AudioSource> TriangleGenerator::copy() {
    return std::make_unique<TriangleGenerator>(*this);
}

std::unique_ptr<TriangleGenerator> TriangleGenerator::Create(std::istream& str, const int, const MakeFlags&) {
    return std::make_unique<TriangleGenerator>(str);
}
