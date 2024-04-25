#ifndef L00PR_GENERATOR_NOISE
#define L00PR_GENERATOR_NOISE

#include "generator.hpp"

class NoiseGenerator : public Generator {
public:
    NoiseGenerator(Interpolated<double> gain = 1) : Generator(1, gain, 0.0) {}
    NoiseGenerator(std::istream& str);
    NoiseGenerator(const NoiseGenerator& g) : Generator(g) {}
    double getSample(double, double) { return double(rand())/RAND_MAX;}
    Generator* copy() { return new NoiseGenerator(*this); }
};

#endif

