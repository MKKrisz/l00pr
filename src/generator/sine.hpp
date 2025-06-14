#ifndef L00PR_GENERATORS_SINE
#define L00PR_GENERATORS_SINE

#include "generator.hpp"

/// <summary> 
/// Generates a sine wave 
/// Syntax: sine or sine([frequency_multiplier] [gain] [phase_offset])
/// </summary>
class SineGenerator :public Generator {
public: 
    SineGenerator(Interpolated<double> amplitude = 1.0f,
                  Interpolated<double> phasemul = 1.0f, 
                  Interpolated<double> offset = 0.0f);
    SineGenerator(const SineGenerator& g);

    SineGenerator(std::istream& stream);
    double getSample(double, double) override;
    std::unique_ptr<AudioSource> copy() override;
    std::string ToString() const override { return "Sine"; }


    static std::unique_ptr<SineGenerator> Create(std::istream& stream, const int, const MakeFlags&);
};


#endif
