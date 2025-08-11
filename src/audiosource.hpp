#ifndef L00PR_AUDIOSOURCE_H
#define L00PR_AUDIOSOURCE_H

#include <memory>
#include <vector>
#include <iostream>
#include <optional>

#include "util.hpp"
#include "exceptions/parse_error.hpp"
#include "string_convertible.hpp"
#include "parseable_base.hpp"

class Source;
class Note;
class SourceRef;

// TODO: Make this a vector of the names of valid generators to be built
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

struct AS_Metadata : public Metadata<Source*, const int, const MakeFlags&>{
public:
    std::string syntax;
    std::string desc;
    AS_Metadata(const char* kw, std::function<Source*(std::istream&, const int, const MakeFlags&)> func, const char* syn, const char* desc) 
        : Metadata(kw, func), syntax(syn), desc(desc) {};
    std::string ToString() const override;
};

/// <summary> Base class for filters and generators </summary>
class Source : public StringConvertible, public virtual Writeable {
protected:
    std::string m_label;

    /// <summary> Ending point for feedback loops. </summary>
    double m_feedback;

    /// <summary> Accumulates generators' generated values for this sample before sending it through the filter chain </summary>
    double m_accumulator;
    double m_prev_sample = 0;

    /// <summary> Returns (and then resets) the accumulator value </summary>
    double getAccumulator() {
        m_prev_sample = m_accumulator;
        m_accumulator = 0;
        return m_prev_sample;
    }

    // Base constructors for subclasses.
    Source(const Source& src) : m_label(src.m_label), m_feedback(), m_accumulator(0) {}
    Source() : m_feedback(0), m_accumulator(0) {}
public:

    void label(const std::string& str) { m_label = str; }
    const std::string& label() { return m_label; }

    /// <summary> Function to call to handle the starting of notes </summary>
    virtual void addNote(std::unique_ptr<Note>) = 0;

    /// <summary> Handles receiving feedback values. </summary>
    /// <remarks> 
    /// This implementation is for generator-like sources, since they are the 
    /// endpoint of a filter chain, thus have nowhere to send the feedback.
    /// Filters should overload this, since they can implement that kind of functionality
    /// </remarks>
    virtual void recvFeedback(double val, size_t) {
        m_feedback = val;
    }

    virtual std::string ToString() const { return "AudioSource"; }

    /// <summary> Returns the endpoint of the filter chain. Unused. </summary>
    virtual Source* getBase() {return this;}

    /// <summary> Forces a sample into the sample accumulator </summary>
    virtual void addSample(double sample) {
        m_accumulator += sample;
    }

    virtual double getFrequencyMultiplier(double, int) { return 1; }

    /// <summary> Sends accumulated sample values through the filter chain to be processed </summary>
    /// <remarks> For filters, this function should end up processing the samples </remarks> 
    virtual double getSample(int) { return getAccumulator() + m_feedback; }
    virtual double getLastSample() { return m_prev_sample; }
    virtual double getSingleSample(double phase, double t, int srate) = 0;

    /// <summary> Creates a heap-allocated copy of this src </summary>
    virtual std::unique_ptr<Source> copy() = 0;
    virtual std::vector<SourceRef*> getSourceRefs() { return {}; }
    virtual std::vector<Source*> getLabeled();
    
    /// <summary> Creates a sample and adds it to the accumulator </summary>
    /// <remarks> For filters, should just send the action deeper into the chain </remarks>
    virtual void operator()(double phase, double t, int srate, double extmul) = 0;
    
    /// <summary> Copy assignment operator </summary>
    Source& operator=(const Source&) = default;

    virtual ~Source() { /* nothing */ };

    /// <summary> Parser function for AudioSources </summary>
    /// <returns> A heap-allocated AudioSource value </summary>
    static std::unique_ptr<Source> Make(std::istream&, const int = 44100, const MakeFlags& = MakeFlags::all);

    static Source* getByName(const std::vector<Source*>&, const std::string&);
    static Source* getByName(const std::vector<std::unique_ptr<Source>>&, const std::string&);
};

class SourceRef : public Source {
public:
    SourceRef(const std::string& label, const std::string& err_str = "");
    SourceRef(size_t id, const std::string& err_str = "");
    SourceRef(const SourceRef& other) = default;
    std::pair<std::string, size_t> getWanted() const;
    const std::string& getError() const;
    void setSource(Source* src);
    Source* getSource();
    bool resolved() const;
    bool labeled() const;

    void addNote(std::unique_ptr<Note> note) override;
    void addSample(double sample) override;
    std::vector<SourceRef*> getSourceRefs() override;
    double getFrequencyMultiplier(double t, int srate) override;
    double getSample(int srate) override;
    double getLastSample() override;
    double getSingleSample(double phase, double t, int srate) override;

    std::unique_ptr<Source> copy() override;
    void operator()(double phase, double t, int srate, double extmul) override;
    std::string ToString() const override;
    void Write(std::ostream&) const override;


private:
    std::string wanted_label;
    size_t wanted_id;
    std::string err_str;
    Source* src = nullptr;

    void assert_resolved();
};

#endif
