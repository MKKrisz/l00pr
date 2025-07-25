#include "square.hpp"
#include "value.hpp"

SquareGenerator::SquareGenerator(Interpolated<double> amplitude, 
        Interpolated<double> phasemul, 
        Interpolated<double> offset, 
        Interpolated<double> duty) 
    : Generator(phasemul, amplitude, offset), m_dutyCycle(std::make_unique<ValueGenerator>(duty)) {}

double SquareGenerator::getSample(double p, double t, int srate) {
    double fm = m_dutyCycle->getFrequencyMultiplier(t, srate);
    (*m_dutyCycle)(t*fm, t, srate, 1);
    return fmod(p, 1)>m_dutyCycle->getSample(srate)? 1 : -1;
}

SquareGenerator::SquareGenerator(const SquareGenerator& g) 
    : Generator(g), m_dutyCycle(g.m_dutyCycle->copy()) {}

SquareGenerator::SquareGenerator(std::istream& stream, int srate) : Generator(stream, srate), m_dutyCycle(std::make_unique<ValueGenerator>(0.5)) {
    if(shouldBeDefault) { 
        return;
    }
    if((stream >> skipws).peek() != ')') {
        m_dutyCycle = Source::Make(stream, srate, MakeFlags::all);
    }
    stream >> expect(')');
}

std::unique_ptr<Source> SquareGenerator::copy() { 
    return std::make_unique<SquareGenerator>(*this);
}

std::unique_ptr<SquareGenerator> SquareGenerator::Create(std::istream& str, const int srate, const MakeFlags&) {
    return std::make_unique<SquareGenerator>(str, srate);
}
