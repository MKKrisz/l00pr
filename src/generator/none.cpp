#include "none.hpp"

NoGenerator::NoGenerator() {}
double NoGenerator::getSample(double, double) { return 0; }
Generator* NoGenerator::copy() {return new NoGenerator();}
