#ifndef L00PR_GENERATOR_H
#define L00PR_GENERATOR_H

#include "../audiosource.hpp"
#include "../interpolated.hpp"

/// <summary> AudioSource that actually generates samples </summary>
class Generator : public AudioSource, public Parseable<AudioSource*, AS_Metadata, const int, const MakeFlags&> {
protected:
    /// <summary> The frequency multiplier that should be applied </summary>
    Interpolated<double> m_phasemul;

    /// <summary> The gain multiplier that should be applied </summary>
    Interpolated<double> m_gain;

    /// <summary> The phase offset that should be applied </summary>
    Interpolated<double> m_phaseoffset;

    // Base constructors for subclasses
    Generator(Interpolated<double> mul = 1.0f, Interpolated<double> gain = 1.0f, Interpolated<double> offs = 0.0f)
        : AudioSource(), m_phasemul(mul), m_gain(gain), m_phaseoffset(offs) {}

    Generator(const Generator& g) 
        : AudioSource(g), m_phasemul(g.m_phasemul), m_gain(g.m_gain), m_phaseoffset(g.m_phaseoffset) {}
    
    Generator(std::istream&);

    /// <summary> Value that indicates that there will be no new arguments to this generator </summary>
    bool shouldBeDefault;
public: 
    static void Init();
    
    /// <summary> Gets a sample of this generator </summary>
    /// <param name="phase"> A value going from 0 to 1 </param>
    /// <param name="t"> A timestamp so that the member values of type Interpolated<double> can be applied properly </param>
    virtual double getSample(double phase, double t) = 0;

    /// <summary> Generates the sample from this generator with all the modifiers applied, then adds that to the accumulator </summary>
    virtual void operator()(size_t noteId, double delta, double t, double srate, double extmul) override;

    virtual ~Generator() {}

    virtual std::string ToString() override { return "Generator"; }

    static std::string getFormattedMetadata();
};

#endif
