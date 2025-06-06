#include "square.hpp"

SquareGenerator::SquareGenerator(Interpolated<double> amplitude, 
        Interpolated<double> phasemul, 
        Interpolated<double> offset, 
        Interpolated<double> duty) 
    : Generator(phasemul, amplitude, offset), m_dutyCycle(duty) {}

double SquareGenerator::getSample(double p, double t) {
    return p>m_dutyCycle(t)? 1 : -1;
}

SquareGenerator::SquareGenerator(const SquareGenerator& g) 
    : Generator(g), m_dutyCycle(g.m_dutyCycle) {}

SquareGenerator::SquareGenerator(std::istream& stream) : Generator(stream), m_dutyCycle(0.5) {
    if(shouldBeDefault) { 
        parse_lb(stream);
        return;
    }
    if((stream >> skipws).peek() != ')') {
        m_dutyCycle.Clear();
        stream >> m_dutyCycle;
    }
    stream >> expect(')');
    parse_lb(stream);
}

std::unique_ptr<AudioSource> SquareGenerator::copy() { 
    return std::make_unique<SquareGenerator>(*this);
}

std::unique_ptr<SquareGenerator> SquareGenerator::Create(std::istream& str, const int, const MakeFlags&) {
    return std::make_unique<SquareGenerator>(str);
}
