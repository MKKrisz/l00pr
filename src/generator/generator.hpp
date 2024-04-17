#ifndef L00PR_GENERATOR_H
#define L00PR_GENERATOR_H

#include "../audiosource.h"
#include "../interpolated.hpp"

class Generator : AudioSource {
protected:
    /// The gain multiplier that should be applied
    Interpolated<double> m_gain = 1.0f;

    /// The phase offset that should be applied
    Interpolated<double> m_phaseoffset = 0.0f;

    /// The frequency multiplier that should be applied 
    /// Note that the implementation is kind of wonky and not the greatest
    Interpolated<double> m_phasemul = 1.0f;
    
    /// The generator phases
    std::vector<double> phases;

public: 
    /// Gets a sample of this generator
    /// @param phase: A value going from 0 to 1
    /// @param t: A timestamp so that the member values of type 
    ///          Interpolated<double> can be applied porperly
    virtual double getSample(double phase, double t) = 0;

    virtual inline void addPhase() {
        phases.emplace_back(0);
    }

    virtual inline void removePhase(int id) {
        phases.erase(std::next(phases.begin(), id));
    }

    inline std::vector<double> getPhases() { return phases; }
    inline void setPhases(std::vector<double>& p) { phases = p;}

    virtual Generator* copy() = 0;

    /// Returns the sample from this generator with all the modifiers applied.
    /// Internally calls "getSample()"
    virtual double operator()(int noteId, double delta, double t);

    virtual ~Generator() {}
};

/// Creates a Generator from any given input stream
/// Remarks: out is a fouble pointer because pointer references are not a
///              thing, you cannot allocate at a specific address, and the 
///              value must be able to be accessedd from the outside
std::istream& operator>> (std::istream& stream, Generator** out);
#endif
