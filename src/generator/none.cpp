#include "none.hpp"

NoGenerator::NoGenerator() {}
double NoGenerator::getSample(double phase, double t) { return 0; }
Generator* NoGenerator::copy() {return new NoGenerator();}
