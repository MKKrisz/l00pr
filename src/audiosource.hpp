#ifndef L00PR_AUDIOSOURCE_H
#define L00PR_AUDIOSOURCE_H

#include <vector>
#include <istream>

/// <summary> Helper struct that specifies what kind of sources to generate when calling `AudioSource::Make()` </summary>
struct MakeFlags {
    /// <summary> If set, AudioSource::Make() will be allowed to make filters </summary>
    bool filters;

    /// <summary> If set, AudioSource::Make() will be allowed to make gnerators </summary>
    bool generators;

    const static MakeFlags all;
    const static MakeFlags onlyFilters;
    const static MakeFlags onlyGenerators;
};

/// <summary> Base class for filters and generators </summary>
class AudioSource {
protected:
    
    /// <summary> Stores the current phases of generators </summary>
    std::vector<double> phases;

    /// <summary> Ending point for feedback loops. </summary>
    double feedback;

    /// <summary> Accumulates generators' generated values for this sample before sending it through the filter chain </summary>
    double accumulator;

    // Base constructors for subclasses.
    AudioSource(const AudioSource& src) : phases(src.phases), feedback(), accumulator() {}
    AudioSource() : phases(), feedback(0), accumulator() {}
public:

    /// <summary> Handles receiving feedback values. </summary>
    /// <remarks> 
    /// This implementation is for generator-like sources, since they are the 
    /// endpoint of a filter chain, thus have nowhere to send the feedback.
    /// Filters should overload this, since they can implement that kind of functionality
    /// </remarks>
    virtual void recvFeedback(double val, size_t) {
        feedback = val;
    }

    /// <summary> Returns the endpoint of the filter chain. Unused. </summary>
    virtual AudioSource* getBase() {return this;}

    /// <summary> Adds a phase value to this source </summary>
    /// <remarks> Filters may want to overload this since in most cases, they don't need the phase values to function. </remarks>
    virtual inline void addPhase() {
        phases.emplace_back(0);
    }

    /// <summary> Removes the phase value at index `id` from this source </summary>
    /// <remarks> Filters may want to overload this since in most cases, they don't need the phase values to function. </remarks>
    virtual inline void removePhase(int id) {
        phases.erase(std::next(phases.begin(), id));
    }

    /// <summary> Returns this source's phases </summary>
    inline std::vector<double> getPhases() { return phases; }

    /// <summary> Sets this source's phase values to `p` </summary>
    inline void setPhases(std::vector<double>& p) { phases = p;}

    /// <summary> Sends accumulated sample values through the filter chain to be processed </summary>
    /// <remarks> For filters, this function should end up processing the samples </remarks> 
    virtual double calc() {
        double ret = accumulator + feedback;
        accumulator = 0;
        return ret;
    }

    /// <summary> Creates a heap-allocated copy of this src </summary>
    virtual AudioSource* copy() = 0;
    
    /// <summary> Creates a sample and adds it to the accumulator </summary>
    /// <remarks> For filters, should just send the action deeper into the chain </remarks>
    virtual void operator()(int noteId, double delta, double t, double extmul, double srate) = 0;
    
    /// <summary> Copy assignment operator </summary>
    AudioSource& operator=(const AudioSource&) = default;

    virtual ~AudioSource() {};

    /// <summary> Parser function for AudioSources </summary>
    /// <returns> A heap-allocated AudioSource value </summary>
    static AudioSource* Make(std::istream&, const int = 44100, const MakeFlags& = MakeFlags::all);
};

#endif
