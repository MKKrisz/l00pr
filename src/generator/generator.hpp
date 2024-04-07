#ifndef L00PR_GENERATOR_H
#define L00PR_GENERATOR_H

#include "../audiosource.h"
#include "../interpolated.hpp"

class Generator : AudioSource {
protected:
    /// The gain multiplier that should be applied
    Interpolated<float> m_gain = 1.0f;

    /// The phase offset that should be applied
    Interpolated<float> m_phaseoffset = 0.0f;

    /// The frequency multiplier that should be applied 
    /// Note that the implementation is kind of wonky and not the greatest
    Interpolated<float> m_phasemul = 1.0f;
    
    /// The generator phases
    std::vector<float> phases;

public: 
    /// Gets a sample of this generator
    /// @param phase: A value going from 0 to 1
    /// @param t: A timestamp so that the member values of type 
    ///          Interpolated<float> can be applied porperly
    virtual float getSample(float phase, float t) = 0;

    virtual inline void addPhase() {
        phases.emplace_back(0);
    }

    virtual inline void removePhase(int id) {
        phases.erase(std::next(phases.begin(), id));
    }

    inline std::vector<float> getPhases() { return phases; }
    inline void setPhases(std::vector<float> p) { phases = p;}

    virtual Generator* copy() = 0;

    /// Returns the sample from this generator with all the modifiers applied.
    /// Internally calls "getSample()"
    virtual float operator()(int noteId, float delta, float t);

    virtual ~Generator() {}
};

/// Creates a Generator from any given input stream
/// Remarks: out is a fouble pointer because pointer references are not a
///              thing, you cannot allocate at a specific address, and the 
///              value must be able to be accessedd from the outside
std::istream& operator>> (std::istream& stream, Generator** out);
#endif
