#ifndef L00PR_GENERATORS_SQUARE
#define L00PR_GENERATORS_SQUARE

#include "generator.hpp"

class SquareGenerator :public Generator {
    Interpolated<float> m_dutyCycle;

public: 
    SquareGenerator(Interpolated<float> amplitude = 1.0f,
                    Interpolated<float> phasemul = 1.0f,
                    Interpolated<float> offset = 0.0f,
                    Interpolated<float> duty = 0.5f);

    SquareGenerator(std::istream& stream);
    float getSample(float phase, float t);
    Generator* copy();
};

#endif
