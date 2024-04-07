#ifndef L00PR_GENERATORS_SINE
#define L00PR_GENERATORS_SINE

#include "generator.hpp"

class SineGenerator :public Generator {
public: 
    SineGenerator(Interpolated<float> amplitude = 1.0f,
                  Interpolated<float> phasemul = 1.0f, 
                  Interpolated<float> offset = 0.0f);

    SineGenerator(std::istream& stream);
    float getSample(float phase, float t);
    Generator* copy();
};

#endif
