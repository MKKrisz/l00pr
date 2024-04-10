#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "../interpolated.hpp"

/// Contains note stuff
class Note {
    
    /// The length of the note measured in seconds
    double len = 1;

    /// The frequency of the note measured in Hz
    Interpolated<double> freq = 0.0;

    /// The amplitude of the note
    Interpolated<double> ampl = 1;

    /// The amount that is completed measured in seconds
    double done = 0;

public:
    /// Returns the current phase value, then increments it according to the 
    /// sample rate, the frequency and the multiplier value.
    float getDelta(int srate = 48000);

    /// Gets the "done" value
    inline float getTime() { return done; }
    inline float getLen()  { return len; }

    /// Returns the amplitude
    float getAmplitude(float t);
    float getAmplitude();

    /// Returns the frequency
    float getFreq(float t);
    float getFreq();

    /// Returns true if done > len
    virtual bool isComplete();

    Note(float l, Interpolated<double> freq, Interpolated<double> amp);
    Note(std::istream& str, double bpm);
};


#endif
