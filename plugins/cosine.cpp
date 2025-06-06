#include "cosine.hpp"

#include <cmath>

#ifdef _WIN32
#define M_PI 3.141592653589793238
#endif
CosineGenerator::CosineGenerator(Interpolated<double> phasemul,
        Interpolated<double> amplitude, 
        Interpolated<double> offset) : Generator(phasemul, amplitude, offset){}


CosineGenerator::CosineGenerator(const CosineGenerator& g) : Generator(g){}

double CosineGenerator::getSample(double p, double) {
    return std::cos(p*2*M_PI);
}

CosineGenerator::CosineGenerator(std::istream& stream) : Generator(stream) {
    if(shouldBeDefault) { 
        parse_lb(stream);
        return;
    }
    if((stream >> skipws).peek() != ')') 
        throw parse_error(stream, "Excepted ')'");
    stream.get();
    parse_lb(stream);
}

std::unique_ptr<AudioSource> CosineGenerator::copy() { 
    return std::make_unique<CosineGenerator>(*this);
}

std::unique_ptr<CosineGenerator> CosineGenerator::Create(std::istream& stream, const int, const MakeFlags&) {
    return std::make_unique<CosineGenerator>(stream);
}

extern "C" { 
void Init(Plugin* p) {
    p->name = "Cosine";
    p->custom_generator_meta = {
        Gen_Metadata("cosine", CosineGenerator::Create, "[freq] [ampl] [phase_offset]", "Creates a cosine wave")
    };
}
}
