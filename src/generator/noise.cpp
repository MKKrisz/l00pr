#include "noise.hpp"

NoiseGenerator::NoiseGenerator(std::istream& stream) : Generator() {
    stream >> skipws;
    if(stream.peek() != '(') return;
    stream.get();
    stream >> skipws;
    if(stream.peek() == ')') return;
    stream >> m_gain;
    stream >> expect(')');
}

std::unique_ptr<NoiseGenerator> NoiseGenerator::Create(std::istream& str, const int, const MakeFlags&) {
    return std::make_unique<NoiseGenerator>(str);
}
