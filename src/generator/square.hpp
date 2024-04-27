#ifndef L00PR_GENERATORS_SQUARE
#define L00PR_GENERATORS_SQUARE

#include "generator.hpp"

/// <summary> 
/// Generates a square wave.
/// Syntax: square (for default args)
/// or square([freq_multiplier] [amplitude] [phase_offset] [duty_cycle])
/// </summary>
class SquareGenerator :public Generator {
    /// <summary> A value between 0 and 1, represents the point where the generator switches from high to low </summary>
    Interpolated<double> m_dutyCycle;

public: 
    SquareGenerator(Interpolated<double> amplitude = 1.0f,
                    Interpolated<double> phasemul = 1.0f,
                    Interpolated<double> offset = 0.0f,
                    Interpolated<double> duty = 0.5f);

    SquareGenerator(std::istream& stream);
    SquareGenerator(const SquareGenerator& g);
    double getSample(double phase, double t);
    AudioSource* copy();
};

#endif
