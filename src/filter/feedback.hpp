#ifndef L00PR_FILTER_FEEDBACK
#define L00PR_FILTER_FEEDBACK

#include "filter.hpp"
#include "../util.hpp"

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
    Filter* fbFilter = nullptr;

public:
    // cctors
    Feedback(size_t depth = 0, Filter* fb = nullptr, AudioSource* src = nullptr)
        : Filter(src), depth(depth), fbFilter(fb) {}

    Feedback(const Feedback& fb) : 
        Filter(fb), depth(fb.depth), fbFilter(fb.fbFilter != nullptr ? (Filter*)fb.fbFilter->copy() : nullptr) {}

    Feedback(std::istream& str, int srate, const MakeFlags& flags = MakeFlags::all) : Filter(), fbFilter(nullptr) {
        str >> expect('(') >> depth >> skipws;
        if(str.peek() != ')') {
            fbFilter = (Filter*)AudioSource::Make(str, srate, MakeFlags::onlyFilters);
        }
        str >> expect(')') >> expect('{');
        src = AudioSource::Make(str, srate, flags);
        str >> expect('}');
    }

    /// <summary> Sends back the sample modified by `fbFilter` `depth` filters deep, then returns the original, unmodified sample </summary>
    double filter(double sample, double delta, double t, double srate) {
        double fbs = fbFilter == nullptr? sample : fbFilter->filter(sample, delta, t, srate);
        src->recvFeedback(fbs, depth);
        return sample;
    }

    AudioSource* copy() { return new Feedback(*this); }
};

#endif
