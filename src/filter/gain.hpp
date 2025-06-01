#ifndef L00PR_FILTER_GAIN
#define L00PR_FILTER_GAIN

#include "filter.hpp"
#include "../util.hpp"

/// <summary> 
/// Simple gain filter. Multiplies the sample by some value.
/// Syntax: gain(<amplitude>) {[src]}
/// </summary>
class GainFilter : public Filter {

    /// <summary> The value to multiply by </summary>
    double gain;
public:
    // cctors
    GainFilter(double gain = 1, AudioSource* src = nullptr) : Filter(src), gain(gain) {}

    GainFilter(const GainFilter& f) : Filter(f), gain(f.gain) {}

    GainFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> gain >> expect(')') >> skipws;
        if(str.peek() == '{'){
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }

    /// <summary> Multiplies the sample value by gain </summary>
    inline double filter(double sample, double, double, double) { return sample * gain; }
    std::unique_ptr<AudioSource> copy() {return std::make_unique<GainFilter>(*this); }
    inline GainFilter& operator=(const GainFilter&) = default;
    std::string ToString() {return Filter::ToString() + "(" + std::to_string(gain) + ")";}
    static std::unique_ptr<GainFilter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return make_unique<GainFilter>(str, srate, flags);
    }
};

#endif
