#ifndef L00PR_GENERATORS_TRIANGLE
#define L00PR_GENERATORS_TRIANGLE

#include "generator.hpp"

/// <summary> 
/// Generates a Triangle wave 
/// Syntax: triangle
/// or triangle([freq_multiplier] [gain] [phase_offset] [peak])
/// </summary>
/// <remarks> In this context, Sawtooth waves are also triangles </summary>
class TriangleGenerator :public Generator {
    /// <summary> A value between 0 and 1, marks the point where the wave peaks. </summary>
    Interpolated<double> m_peak;
    
public:
    TriangleGenerator(Interpolated<double> amplitude = 1.0f, 
                      Interpolated<double> phasemul = 1.0f, 
                      Interpolated<double> offset = 0.0f, 
                      Interpolated<double> peak = 0.5f);
    TriangleGenerator(std::istream& stream);
    TriangleGenerator(const TriangleGenerator&);

    double getSample(double phase, double t) override;
    std::unique_ptr<AudioSource> copy() override;
    std::string ToString() const override { return "Triangle"; }

    void Write(std::ostream& str) const override { 
        str << "triangle(";
        WriteBaseParams(str);
        str << "  "; 
        m_peak.Write(str);
        str << ") ";
        WriteLengthBounds(str);
    }

    static std::unique_ptr<TriangleGenerator> Create(std::istream& str, const int, const MakeFlags&);
};

#endif
