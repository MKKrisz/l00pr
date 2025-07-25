#include "none.hpp"

NoGenerator::NoGenerator() : Generator() {}
double NoGenerator::getSample(double, double, int) { return 0; }
std::unique_ptr<Source> NoGenerator::copy() {return std::make_unique<NoGenerator>();}

std::unique_ptr<NoGenerator> NoGenerator::Create(std::istream&, const int, const MakeFlags&) {
    return std::make_unique<NoGenerator>();
}
