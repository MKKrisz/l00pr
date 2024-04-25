#ifndef L00PR_FILTER_GAIN
#define L00PR_FILTER_GAIN

#include "filter.hpp"
#include "../util.hpp"

class GainFilter : public Filter {
    double gain;
public:
    GainFilter(double gain = 1, AudioSource* src = nullptr) : Filter(src), gain(gain) {}
    GainFilter(const GainFilter& f) : Filter(f), gain(f.gain) {}
    GainFilter(std::istream& str, int srate, MakeFlags& flags = MakeFlags::all) {
        double g;
        AudioSource* src = nullptr;
        str >> expect('(') >> g >> expect(')') >> skipws;
        if(str.peek() == '{'){
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
        *this = GainFilter(g, src);
    }
    inline double filter(double sample, double, double, double) { return sample * gain; }
    AudioSource* copy() {return new GainFilter(*this); }
    inline GainFilter& operator=(const GainFilter&) = default;
};

#endif
