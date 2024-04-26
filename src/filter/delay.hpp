#ifndef L00PR_FILTER_DELAY
#define L00PR_FILTER_DELAY

#include <vector>
#include "filter.hpp"
#include "../util.hpp"

//TODO: allow sample-based delaying
class DelayFilter : public Filter {
    std::vector<double> sbuf;
    size_t bufId = 0;
public:
    DelayFilter(double t, AudioSource* src, int srate) : Filter(src), sbuf(t*srate, 0) {}

    DelayFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        double t;
        str >> expect('(') >> t >> expect(')') >> skipws;
        if(str.peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
        sbuf = std::vector<double>(t*srate, 0);
    }
    DelayFilter(const DelayFilter& f) : Filter(f), sbuf(f.sbuf), bufId(f.bufId){}
    double filter(double sample, double, double, double) {
        sbuf[bufId++] = sample;
        bufId = bufId % sbuf.size();
        return sbuf[bufId];
    }
    AudioSource* copy() {return new DelayFilter(*this); }
    DelayFilter& operator=(const DelayFilter& f) {
        if(this == &f) return *this;
        Filter::operator=(f);
        sbuf = f.sbuf;
        bufId = f.bufId;
        return *this;
    };
};

#endif

