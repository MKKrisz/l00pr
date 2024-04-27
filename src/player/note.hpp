#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "../interpolated.hpp"
#include "../freq.hpp"

/// <summary> 
/// Data for the generators to play.
/// Syntax: <frequency> <length> [amplitude]
/// </summary>
class Note {
    
    /// <summary> The length of the note measured in seconds </summary>
    double len = 1;

    /// <summary> The frequency of the note measured in Hz </summary>
    Interpolated<Frequency> freq = Frequency(0.0);

    /// <summary> The amplitude of the note </summary>
    Interpolated<double> ampl = 1;

    /// <summary> The amount that is completed measured in seconds </summary>
    double done = 0;

public:
    /// <summary> Calculates the next phase delta value </summary>
    /// <returns>
    /// the current phase value, then increments it according to the 
    /// sample rate, the frequency and the multiplier value.
    /// </summary>
    double getDelta(int srate = 48000);

    /// <summary> Gets how much has been elapsed since the start of this note in seconds </summary>
    inline double getTime() { return done; }

    /// <summary> Gets the length of this note </summary>
    inline double getLen()  { return len; }

    /// <summary> Gets the string representation of this note </summary>
    inline std::string toString() {
        return "[" + freq[0].second.getName() + (freq.Size() == 1 ? "       " : "-/\\/-- ") + std::to_string(len)  + "    " + std::to_string(ampl[0].second) + "] ";
    }

    /// <summary> Gets the amplitude value at timestamp t </summary>
    double getAmplitude(double t);

    /// <summary> Gets the current amplitude value </summary>
    double getAmplitude();

    /// <summary> Gets the frequency value at timestamp t </summary>
    double getFreq(double t);
    /// <summary> Gets the current frequency value </summary>
    double getFreq();
    
    /// <summary> Checks wether the note is completed </summary>
    /// <remarks> A little more complicated than just "done > len", as that would chop off the end of the note... </remarks>
    virtual bool isComplete();

    // cctor
    Note(double l, Interpolated<Frequency> freq, Interpolated<double> amp);

    // Yes, bad design, according to prog2 auditor
    /// <summary> Parser for notes </summary>
    Note(std::istream& str, double bpm);
};

/// <summary> Writes the string representation of `n` onto `str` </summary>
std::ostream& operator<<(std::ostream& str, Note& n); 

#endif
