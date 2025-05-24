#include "none.hpp"

NoGenerator::NoGenerator() : Generator() {}
double NoGenerator::getSample(double, double) { return 0; }
NoGenerator* NoGenerator::copy() {return new NoGenerator();}

NoGenerator* NoGenerator::Create(std::istream& str, const int, const MakeFlags&) {
    return new NoGenerator();
}
