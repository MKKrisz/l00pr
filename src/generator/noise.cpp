#include "noise.hpp"

NoiseGenerator::NoiseGenerator(std::istream& stream, int srate) : Generator() {
    stream >> skipws;
    if(stream.peek() != '(') return;
    stream.get();
    stream >> skipws;
    if(stream.peek() == ')') return;

    m_gain = Source::Make(stream, srate, MakeFlags::all);

    stream >> expect(')');
}

std::unique_ptr<NoiseGenerator> NoiseGenerator::Create(std::istream& str, const int srate, const MakeFlags&) {
    return std::make_unique<NoiseGenerator>(str, srate);
}
