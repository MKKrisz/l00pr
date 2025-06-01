#include "none.hpp"

NoGenerator::NoGenerator() : Generator() {}
double NoGenerator::getSample(double, double) { return 0; }
std::unique_ptr<AudioSource> NoGenerator::copy() {return std::make_unique<NoGenerator>();}

std::unique_ptr<NoGenerator> NoGenerator::Create(std::istream& str, const int, const MakeFlags&) {
    return std::make_unique<NoGenerator>();
}
