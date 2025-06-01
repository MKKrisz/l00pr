#include "sine.hpp"

#include <cmath>

#ifdef _WIN32
#define M_PI 3.141592653589793238
#endif

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

std::unique_ptr<AudioSource> SineGenerator::copy() { 
    return std::make_unique<SineGenerator>(*this);
}

std::unique_ptr<SineGenerator> SineGenerator::Create(std::istream& stream, const int, const MakeFlags&) {
    return std::make_unique<SineGenerator>(stream);
}
