#ifndef L00PR_FILTER_FEEDBACK
#define L00PR_FILTER_FEEDBACK

#include "filter.hpp"
#include "../util.hpp"
#include "dummy.hpp"

/// <summary> 
/// Filter used to initiate feedback loops
/// Syntax: feedback(<depth> [feedback_filter_chain]) {[source]}
/// Where `depth` is an unsigned integer describing how deep the sample needs to go into `source` (assuming it's a filter chain)
/// and `feedback_filter_chain` is a generatorless filter chain
/// Ex.: in `feedback(1 gain(0.3)) { delay(0.1) { sine(1 0.5) } }` could be represented as follows:
///           +<-gain-<-+
///           V         A
///     sine--+>-delay--+>--->
/// </summary>
class Feedback : public Filter {

    /// <summary> A value representing how deep the samples need to go </summary>
    size_t depth;

    /// <summary> A filter chain applied to the sample that gets sent back </summary>
    std::unique_ptr<Filter> fbFilter = std::make_unique<DummyFilter>();

public:
    // cctors
    Feedback(size_t depth = 0, Filter* fb = nullptr, AudioSource* src = nullptr)
        : Filter(src), depth(depth), fbFilter(fb) {}

    Feedback(const Feedback& fb) : 
        Filter(fb), depth(fb.depth), fbFilter(fb.fbFilter != nullptr ? (std::unique_ptr<Filter>&&)std::move(fb.fbFilter->copy()) : std::move(std::make_unique<DummyFilter>())) {}

    Feedback(std::istream& str, int srate, const MakeFlags& flags = MakeFlags::all) : Filter(), fbFilter(nullptr) {
        str >> expect('(') >> depth >> skipws;
        if(str.peek() != ')') {
            fbFilter = (std::unique_ptr<Filter>&&)std::move(AudioSource::Make(str, srate, MakeFlags::onlyFilters));
        }
        str >> expect(')') >> skipws;
        if(str.peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }

    /// <summary> Sends back the sample modified by `fbFilter` `depth` filters deep, then returns the original, unmodified sample </summary>
    double filter(double sample, double, double, double) {
        double fbs = sample; 
        if(fbFilter != nullptr) {
            fbFilter->addSample(sample);
            fbs = fbFilter->calc();
        }
        //fbFilter == nullptr? sample : fbFilter->filter(sample, delta, t, srate);
        src->recvFeedback(fbs, depth);
        return sample;
    }
    
    std::string ToString() { return Filter::ToString() + "Feedback"; }

    std::unique_ptr<AudioSource> copy() { return std::make_unique<Feedback>(*this); }
    ~Feedback() {}

    static std::unique_ptr<Feedback> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<Feedback>(str, srate, flags);
    }
};

#endif
