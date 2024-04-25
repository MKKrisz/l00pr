#ifndef L00PR_GENERATORS_SINE
#define L00PR_GENERATORS_SINE

#include "generator.hpp"

class SineGenerator :public Generator {
public: 
    SineGenerator(Interpolated<double> amplitude = 1.0f,
                  Interpolated<double> phasemul = 1.0f, 
                  Interpolated<double> offset = 0.0f);
    SineGenerator(const SineGenerator& g);

    SineGenerator(std::istream& stream);
    double getSample(double, double);
    AudioSource* copy();
};

#endif
