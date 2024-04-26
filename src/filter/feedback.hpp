#ifndef L00PR_FILTER_FEEDBACK
#define L00PR_FILTER_FEEDBACK

#include "filter.hpp"
#include "../util.hpp"

class Feedback : public Filter {
    size_t depth;
    Filter* fbFilter = nullptr;

public:
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

    double filter(double sample, double delta, double t, double srate) {
        double fbs = fbFilter == nullptr? sample : fbFilter->filter(sample, delta, t, srate);
        src->recvFeedback(fbs, depth);
        return sample;
    }

    AudioSource* copy() { return new Feedback(*this); }
};

#endif
