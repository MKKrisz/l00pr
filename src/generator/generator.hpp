#ifndef L00PR_GENERATOR_H
#define L00PR_GENERATOR_H

#include "../audiosource.hpp"
#include "../interpolated.hpp"

class Generator : public AudioSource {
protected:
    /// The frequency multiplier that should be applied 
    /// Note that the implementation is kind of wonky and not the greatest
    Interpolated<double> m_phasemul;

    /// The gain multiplier that should be applied
    Interpolated<double> m_gain;

    /// The phase offset that should be applied
    Interpolated<double> m_phaseoffset;

    Generator(Interpolated<double> mul = 1.0f, Interpolated<double> gain = 1.0f, Interpolated<double> offs = 0.0f)
        : AudioSource(), m_phasemul(mul), m_gain(gain), m_phaseoffset(offs) {}

    Generator(const Generator& g) 
        : AudioSource(g), m_phasemul(g.m_phasemul), m_gain(g.m_gain), m_phaseoffset(g.m_phaseoffset) {}
    
    Generator(std::istream&);
    bool shouldBeDefault;
public: 
    /// Gets a sample of this generator
    /// @param phase: A value going from 0 to 1
    /// @param t: A timestamp so that the member values of type 
    ///          Interpolated<double> can be applied properly
    virtual double getSample(double phase, double t) = 0;

    /// Returns the sample from this generator with all the modifiers applied.
    /// Internally calls "getSample()"
    double operator()(int noteId, double delta, double t, double srate);

    virtual ~Generator() {}

    static bool ValidName(std::string&);
    static size_t LongestName();
    static Generator* Make(std::string&, std::istream&, int, MakeFlags& = MakeFlags::all);
};

#endif
