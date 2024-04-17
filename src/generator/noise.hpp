#ifndef L00PR_GENERATOR_NOISE
#define L00PR_GENERATOR_NOISE

#include "generator.hpp"

class NoiseGenerator : public Generator {
public:
    NoiseGenerator(Interpolated<double> gain = 1) {
        m_gain = gain;
        m_phaseoffset = 0.0;
        m_phasemul = 1;
        phases = std::vector<double>();
    }
    NoiseGenerator(std::istream& str);
    double getSample(double, double) { return double(rand())/RAND_MAX;}
    Generator* copy() { return new NoiseGenerator(m_gain); }
};

#endif

