#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "../interpolated.hpp"
#include "../freq.hpp"

/// Contains note stuff
class Note {
    
    /// The length of the note measured in seconds
    double len = 1;

    /// The frequency of the note measured in Hz
    Interpolated<Frequency> freq = Frequency(0.0);

    /// The amplitude of the note
    Interpolated<double> ampl = 1;

    /// The amount that is completed measured in seconds
    double done = 0;

public:
    /// Returns the current phase value, then increments it according to the 
    /// sample rate, the frequency and the multiplier value.
    double getDelta(int srate = 48000);

    /// Gets the "done" value
    inline double getTime() { return done; }
    inline double getLen()  { return len; }
    inline std::string toString() {
        return "[" + freq[0].second.getName() + (freq.Size() == 1 ? "       " : "-/\\/-- ") + std::to_string(len)  + "    " + std::to_string(ampl[0].second) + "] ";
    }

    /// Returns the amplitude
    double getAmplitude(double t);
    double getAmplitude();

    /// Returns the frequency
    double getFreq(double t);
    double getFreq();
    

    /// Returns true if done > len
    virtual bool isComplete();

    Note(double l, Interpolated<Frequency> freq, Interpolated<double> amp);
    Note(std::istream& str, double bpm);
};

std::ostream& operator<<(std::ostream& str, Note& n); 

#endif
