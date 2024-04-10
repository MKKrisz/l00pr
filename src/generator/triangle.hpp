#ifndef L00PR_GENERATORS_TRIANGLE
#define L00PR_GENERATORS_TRIANGLE

#include "generator.hpp"

class TriangleGenerator :public Generator {
    Interpolated<double> m_peak;
    
public:
    TriangleGenerator(Interpolated<double> amplitude = 1.0f, 
                      Interpolated<double> phasemul = 1.0f, 
                      Interpolated<double> offset = 0.0f, 
                      Interpolated<double> peak = 0.5f);
    TriangleGenerator(std::istream& stream);

    double getSample(double phase, double t);
    Generator* copy();
};

#endif
