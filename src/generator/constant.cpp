#include "constant.hpp"

#include <cmath>

#ifdef _WIN32
#define M_PI 3.141592653589793238
#endif

ConstantGenerator::ConstantGenerator(Interpolated<double> phasemul,
        Interpolated<double> amplitude, 
        Interpolated<double> offset) : Generator(phasemul, amplitude, offset){}


ConstantGenerator::ConstantGenerator(const ConstantGenerator& g) : Generator(g), m_constant(g.m_constant) {}

double ConstantGenerator::getSample(double, double) {
    return m_constant;
}

ConstantGenerator::ConstantGenerator(std::istream& stream) : Generator() {
    if((stream >> skipws).peek() != '('){ return; }
    stream.get();
    stream >> skipws >> m_constant >> expect(')');
}

std::unique_ptr<AudioSource> ConstantGenerator::copy() { 
    return std::make_unique<ConstantGenerator>(*this);
}

std::unique_ptr<ConstantGenerator> ConstantGenerator::Create(std::istream& stream, const int, const MakeFlags&) {
    return std::make_unique<ConstantGenerator>(stream);
}
void ConstantGenerator::Write(std::ostream& str) const {
    str << "constant(" << m_constant << ") ";
    WriteLengthBounds(str);
}
