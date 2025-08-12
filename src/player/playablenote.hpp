#ifndef L00PR_NOTES_PLAYABLE
#define L00PR_NOTES_PLAYABLE

#include "../interpolated.hpp"
#include "../freq.hpp"
#include "../audiosource.hpp"
#include "note.hpp"

class PlayableNoteBase : public Note {
public:
    virtual double getAmplitude(double t) = 0;
    virtual const Interpolated<double>& getAmplitudeOverTime() const = 0;
    virtual void setAmplitudeAt(double t, double value) = 0;
    virtual void setAmplitude(Interpolated<double>&) = 0;
    double getAmplitude() { return getAmplitude(m_done/m_length); }

    virtual double getFreq(double t) = 0;
    virtual const Interpolated<Frequency>& getFreqOverTime() const = 0;
    virtual void setFreqAt(double t, Frequency value) = 0;
    virtual void setFreq(Interpolated<Frequency>&) = 0;
    double getFreq() { return getFreq(m_done/m_length); }

    double GetLen() const override { return m_length; }
    void SetLen(double len) { m_length = len; }
    void ClampLen(double min, double max) {
        if(m_length < min) m_length = min;
        if(m_length > max) m_length = max;
    }
    bool IsComplete() const override { return m_done >= m_length; }


protected:
    double m_done = 0;
    double m_length = 0;
    PlayableNoteBase() : Note() {}
    PlayableNoteBase(double len, double done = 0) : Note(), m_done(done), m_length(len) {}
    PlayableNoteBase(const PlayableNoteBase& n) : Note(), m_done(n.m_done), m_length(n.m_length) {}
};

/// <summary> 
/// Data for the generators to play.
/// Syntax: <frequency> <length> [amplitude]
/// </summary>
class PlayableNote : public PlayableNoteBase{
    /// <summary> The frequency of the note measured in Hz </summary>
    Interpolated<Frequency> m_frequency = Frequency(0.0);

    /// <summary> The amplitude of the note </summary>
    Interpolated<double> m_amplitude = 1;

    /// <summary> The amount that is completed measured in seconds </summary>
    double phase = 0;

public:
    void advance(int srate, double fm = 1.0);

    /// <summary> Calculates the next phase delta value </summary>
    /// <returns>
    /// the current phase value, then increments it according to the 
    /// sample rate, the frequency and the multiplier value.
    /// </summary>
    double getDelta(int srate = 48000);

    /// <summary> Gets how much has been elapsed since the start of this note in seconds </summary>
     double getTime() { return m_done; }

    /// <summary> Gets the length of this note </summary>
     double GetLen() const override { return m_length; }

    /// <summary> Gets the string representation of this note </summary>
     std::string ToString() const override {
        return "[" + m_frequency[0].second.getName() + (m_frequency.Size() == 1 ? "       " : "-/\\/-- ") + std::to_string(m_length)  + "    " + std::to_string(m_amplitude[0].second) + "] ";
    }

    /// <summary> Gets the amplitude value at timestamp t </summary>
    double getAmplitude(double t) override;
    const Interpolated<double>& getAmplitudeOverTime() const override { return m_amplitude; }
    void setAmplitudeAt(double t, double value) override { m_amplitude.Set(t, value); }
    void setAmplitude(Interpolated<double>& ampl) override { m_amplitude = ampl; }


    /// <summary> Gets the frequency value at timestamp t </summary>
    double getFreq(double t) override;
    const Interpolated<Frequency>& getFreqOverTime() const override { return m_frequency; }
    void setFreqAt(double t, Frequency value) override { m_frequency.Set(t, value); } 
    void setFreq(Interpolated<Frequency>& freq) override { m_frequency = freq; } 
    
    /// <summary> Checks whether the note is completed </summary>
    /// <remarks> A little more complicated than just "done > len", as that would chop off the end of the note... </remarks>
    bool IsComplete() const override;

    void SetLength(double l) {m_length = l;}

    void AddSample(Source* p, int srate) override {
        double t = getTime();
        double ampl = PlayableNoteBase::getAmplitude();
        double freq_mul = p->getFrequencyMultiplier(t, srate);
        (*p)(phase, t, srate, ampl);
        advance(srate, freq_mul);
    }

    void clampLength(double min, double max) {
        if(m_length < min) m_length = min;
        if(m_length > max) m_length = max;
    }

    // cctor
    PlayableNote(double l, Interpolated<Frequency> freq, Interpolated<double> amp, double done = 0);
    PlayableNote(const PlayableNote&) = default;

    // Yes, bad design, according to prog2 auditor
    /// <summary> Parser for notes </summary>
    PlayableNote(std::istream& str, double bpm);

    std::unique_ptr<Note> copy() const override {return std::make_unique<PlayableNote>(*this);}

    void Write(std::ostream&) const override;

    static std::unique_ptr<PlayableNote> Create(std::istream&, Tune*, double, bool, int);
};

/// <summary> Writes the string representation of `n` onto `str` </summary>
std::ostream& operator<<(std::ostream& str, PlayableNote& n); 

#endif
