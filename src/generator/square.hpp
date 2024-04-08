#ifndef L00PR_GENERATORS_SQUARE
#define L00PR_GENERATORS_SQUARE

#include "generator.hpp"

class SquareGenerator :public Generator {
    Interpolated<double> m_dutyCycle;

public: 
    SquareGenerator(Interpolated<double> amplitude = 1.0f,
                    Interpolated<double> phasemul = 1.0f,
                    Interpolated<double> offset = 0.0f,
                    Interpolated<double> duty = 0.5f);

    SquareGenerator(std::istream& stream);
    double getSample(double phase, double t);
    Generator* copy();
};

#endif
