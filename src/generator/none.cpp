#include "none.hpp"

NoGenerator::NoGenerator() : Generator() {}
double NoGenerator::getSample(double, double) { return 0; }
NoGenerator* NoGenerator::copy() {return new NoGenerator();}
