#ifndef L00PR_AUDIOSOURCE_H
#define L00PR_AUDIOSOURCE_H

#include <vector>
#include <istream>
#include <optional>

#include "util.hpp"
#include "exceptions/parse_error.hpp"
#include "string_convertible.hpp"
#include "parseable_base.hpp"

class AudioSource;



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

struct AS_Metadata : public Metadata<AudioSource*, const int, const MakeFlags&>{
public:
    std::string syntax;
    std::string desc;
    AS_Metadata(const char* kw, std::function<AudioSource*(std::istream&, const int, const MakeFlags&)> func, const char* syn, const char* desc) 
        : Metadata(kw, func), syntax(syn), desc(desc) {};
};

/// <summary> Base class for filters and generators </summary>
class AudioSource : public StringConvertible {
protected:
    
    /// <summary> Stores the current phases of generators </summary>
    std::vector<double> phases;

    /// <summary> Ending point for feedback loops. </summary>
    double feedback;

    /// <summary> Accumulates generators' generated values for this sample before sending it through the filter chain </summary>
    double accumulator;

    /// <summary> If set, specifies the minimum and maximum lengths a generator can output </summary>
    std::optional<std::pair<double, double>> length_bounds;

    /// <summary> Standard parser for length bounds </summary>
    void parse_lb(std::istream& str);

    /// <summary> Returns (and then resets) the accumulator value </summary>
    double getAccumulator() {
        double ac = accumulator;
        accumulator = 0;
        return ac;
    }

    // Base constructors for subclasses.
    AudioSource(const AudioSource& src) : phases(src.phases), feedback(), accumulator(0), length_bounds(src.length_bounds) {}
    AudioSource() : phases(), feedback(0), accumulator(0), length_bounds() {}
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

    virtual std::string ToString() { return "AudioSource"; }

    /// <summary> Returns the endpoint of the filter chain. Unused. </summary>
    virtual AudioSource* getBase() {return this;}

    /// <summary> Forces a sample into the sample accumulator </summary>
    virtual void addSample(double sample) {
        accumulator += sample;
    }

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

    virtual inline std::optional<std::pair<double, double>> getLengthBounds() {
        return length_bounds;
    }

    /// <summary> Returns this source's phases </summary>
    inline std::vector<double> getPhases() { return phases; }

    /// <summary> Sets this source's phase values to `p` </summary>
    inline void setPhases(std::vector<double>& p) { phases = p;}

    /// <summary> Sends accumulated sample values through the filter chain to be processed </summary>
    /// <remarks> For filters, this function should end up processing the samples </remarks> 
    virtual double calc() { return getAccumulator() + feedback; }

    /// <summary> Creates a heap-allocated copy of this src </summary>
    virtual AudioSource* copy() = 0;
    
    /// <summary> Creates a sample and adds it to the accumulator </summary>
    /// <remarks> For filters, should just send the action deeper into the chain </remarks>
    virtual void operator()(size_t noteId, double delta, double t, double extmul, double srate) = 0;
    
    /// <summary> Copy assignment operator </summary>
    AudioSource& operator=(const AudioSource&) = default;

    virtual ~AudioSource() {};

    /// <summary> Parser function for AudioSources </summary>
    /// <returns> A heap-allocated AudioSource value </summary>
    static AudioSource* Make(std::istream&, const int = 44100, const MakeFlags& = MakeFlags::all);
};

#endif
