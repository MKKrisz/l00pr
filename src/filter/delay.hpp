#ifndef L00PR_FILTER_DELAY
#define L00PR_FILTER_DELAY

#include <vector>
#include "filter.hpp"
#include "../util.hpp"

//TODO: allow sample-based delaying

/// <summary> 
/// Delays the signal
/// Syntax: delay(<seconds>) {src}
/// </summary>
class DelayFilter : public Filter {
    /// <summary> Rotating sample buffer </summary>
    std::vector<double> sbuf;

    /// <summary> Current position in `sbuf` </summary>
    size_t bufId = 0;
    float len = 0;
public:
    // cctors
    DelayFilter(double t, AudioSource* src, int srate) : Filter(src), sbuf(t*srate, 0), len(t) {}

    // parser
    DelayFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> len >> expect(')') >> skipws;
        if(str.peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
        sbuf = std::vector<double>(len*srate, 0);
    }

    DelayFilter(const DelayFilter& f) : Filter(f), sbuf(f.sbuf), bufId(f.bufId), len(f.len){}

    /// <summary> stores `sample` at the current position then returns the next (oldest) sample in sbuf
    double filter(double sample, double, double, double) override {
        sbuf[bufId++] = sample;
        bufId = bufId % sbuf.size();
        return sbuf[bufId];
    }
    std::unique_ptr<AudioSource> copy() override {return std::make_unique<DelayFilter>(*this); }

    std::string ToString() const override { return Filter::ToString() + "Delay(" + std::to_string(sbuf.size()) + ")"; }
    std::string GetNameAndParams() const override { return "delay(" + std::to_string(len) + ')';}

    DelayFilter& operator=(const DelayFilter& f) {
        if(this == &f) return *this;
        Filter::operator=(f);
        sbuf = f.sbuf;
        bufId = f.bufId;
        return *this;
    };
    static std::unique_ptr<DelayFilter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<DelayFilter>(str, srate, flags);
    }
};



#endif

