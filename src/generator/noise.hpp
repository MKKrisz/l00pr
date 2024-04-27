#ifndef L00PR_GENERATOR_NOISE
#define L00PR_GENERATOR_NOISE

#include "generator.hpp"

/// <summary> 
/// Generates random samples between -1 and 1
/// Syntax: random or random([freq_multiplier] [ampl] [phase_offset])
/// </summary>
/// <remarks> Altough they exist, the freq_multiplier and phase_offset variables do nothing </remarks>
class NoiseGenerator : public Generator {
public:

    // cctors
    NoiseGenerator(Interpolated<double> gain = 1) : Generator(1, gain, 0.0) {}
    NoiseGenerator(std::istream& str);
    NoiseGenerator(const NoiseGenerator& g) : Generator(g) {}

    double getSample(double, double t) { return double(rand())/RAND_MAX * m_gain(t);}
    Generator* copy() { return new NoiseGenerator(*this); }
};

#endif

