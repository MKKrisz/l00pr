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
    std::unique_ptr<Source> m_dutyCycle;

public: 
    SquareGenerator(Interpolated<double> amplitude = 1.0f,
                    Interpolated<double> phasemul = 1.0f,
                    Interpolated<double> offset = 0.0f,
                    Interpolated<double> duty = 0.5f);

    SquareGenerator(std::istream& stream, int srate);
    SquareGenerator(const SquareGenerator& g);
    double getSample(double phase, double t, int) override;
    std::unique_ptr<Source> copy() override;
    std::string ToString() const override { return "Square"; }

    void Write(std::ostream& str) const override { 
        str << "square(";
        WriteBaseParams(str);
        str << "  "; 
        m_dutyCycle->Write(str);
        str << ") ";
    }

    static std::unique_ptr<SquareGenerator> Create(std::istream& str, const int, const MakeFlags&);
};


#endif
