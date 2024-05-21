#include "sine.hpp"

#include <cmath>

SineGenerator::SineGenerator(Interpolated<double> phasemul,
        Interpolated<double> amplitude, 
        Interpolated<double> offset) : Generator(phasemul, amplitude, offset){}


SineGenerator::SineGenerator(const SineGenerator& g) : Generator(g){}

double SineGenerator::getSample(double p, double) {
    return std::sin(p*2*M_PI);
}

SineGenerator::SineGenerator(std::istream& stream) : Generator(stream) {
    if(shouldBeDefault) { 
        parse_lb(stream);
        return;
    }
    if((stream >> skipws).peek() != ')') 
        throw parse_error(stream, "Excepted ')'");
    stream.get();
    parse_lb(stream);
}

SineGenerator* SineGenerator::copy() { 
    return new SineGenerator(*this);
}

SineGenerator* SineGenerator::Create(std::istream& stream, const int, const MakeFlags&) {
    return new SineGenerator(stream);
}
