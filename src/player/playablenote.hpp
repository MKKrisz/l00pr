#ifndef L00PR_NOTES_PLAYABLE
#define L00PR_NOTES_PLAYABLE

#include "../interpolated.hpp"
#include "../freq.hpp"
#include "note.hpp"

/// <summary> 
/// Data for the generators to play.
/// Syntax: <frequency> <length> [amplitude]
/// </summary>
class PlayableNote : public Note{
    
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
    inline double GetLen() override { return len; }

    /// <summary> Gets the string representation of this note </summary>
    inline std::string ToString() override {
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
    
    /// <summary> Checks whether the note is completed </summary>
    /// <remarks> A little more complicated than just "done > len", as that would chop off the end of the note... </remarks>
    bool IsComplete() override;

    void AddToPlayer(NotePlayer& p) override {
        if(p.hasBounds()) {
            const auto& bounds = p.getBounds();
            clampLength(bounds.first, bounds.second);
        }
    }

    void AddSample(NotePlayer& p, size_t index, int srate) override {
        double t = getTime();
        double ampl = getAmplitude();
        p(index, getDelta(srate), t, srate, ampl);
    }

    void clampLength(double min, double max) {
        if(len < min) len = min;
        if(len > max) len = max;
    }

    // cctor
    PlayableNote(double l, Interpolated<Frequency> freq, Interpolated<double> amp, double done = 0);
    PlayableNote(const PlayableNote&) = default;

    // Yes, bad design, according to prog2 auditor
    /// <summary> Parser for notes </summary>
    PlayableNote(std::istream& str, double bpm);

    Note* copy() override {return new PlayableNote(*this);}
};

/// <summary> Writes the string representation of `n` onto `str` </summary>
std::ostream& operator<<(std::ostream& str, PlayableNote& n); 

#endif
