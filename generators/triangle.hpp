#ifndef L00PR_GENERATORS_TRIANGLE
#define L00PR_GENERATORS_TRIANGLE

#include "../generator.hpp"

class TriangleGenerator :public Generator {
    Interpolated<float> m_peak;
    
public:
    TriangleGenerator(Interpolated<float> amplitude = 1.0f, 
                      Interpolated<float> phasemul = 1.0f, 
                      Interpolated<float> offset = 0.0f, 
                      Interpolated<float> peak = 0.5f);
    TriangleGenerator(std::istream& stream);

    float getSample(float phase, float t);
    Generator* copy();
};

#endif
