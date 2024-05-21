#ifndef L00PR_FILTER_QUANTIZE
#define L00PR_FILTER_QUANTIZE

#include "filter.hpp"
#include "../util.hpp"

#include <iostream>

/// <summary>
/// Quantizes the signal
/// Syntax: quantize(<bits>) {[src]}
class QuantizeFilter : public Filter {

    /// <summary> The number of bits to quantize by </summary>
    size_t bits;

public:
    // cctors
    QuantizeFilter(size_t bits, AudioSource* src = nullptr) : Filter(src), bits(bits) {}
    QuantizeFilter(const QuantizeFilter& f) : Filter(f), bits(f.bits) {}

    // parser
    QuantizeFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> bits >> expect(')') >> skipws;
        if(str.peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }

    /// <summary> quantizes the floating-point sample by first turning it into PCM64 format, then bitshifting it, then turning it back into floating-point </summary>
    double filter(double sample, double, double, double) {
        unsigned long s_l = (unsigned long)((sample + 1)/ 2 * (double)std::numeric_limits<unsigned long>::max()) >> (64 - bits);
        return (double(s_l) / (std::pow(2, bits)-1) * 2) - 1;
    }

    QuantizeFilter* copy() {return new QuantizeFilter(*this);}
    std::string ToString() { return Filter::ToString() + "q" + std::to_string(bits); }
    static QuantizeFilter* Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return new QuantizeFilter(str, srate, flags);
    } 
};

#endif
