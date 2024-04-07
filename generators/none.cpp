#include "none.hpp"

NoGenerator::NoGenerator() {}
float NoGenerator::getSample(float phase, float t) { return 0; }
Generator* NoGenerator::copy() {return new NoGenerator();}
