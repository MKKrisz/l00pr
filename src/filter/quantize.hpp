#ifndef L00PR_FILTER_QUANTIZE
#define L00PR_FILTER_QUANTIZE

#include "filter.hpp"
#include "../util.hpp"

#include <iostream>

class QuantizeFilter : public Filter {
    size_t bits;

public:
    QuantizeFilter(size_t bits, AudioSource* src = nullptr) : Filter(src), bits(bits) {}
    QuantizeFilter(const QuantizeFilter& f) : Filter(f), bits(f.bits) {}

    QuantizeFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> bits >> expect(')') >> skipws;
        if(str.peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }

    double filter(double sample, double, double, double) {
        unsigned long s_l = (unsigned long)((sample + 1)/ 2 * (double)std::numeric_limits<unsigned long>::max()) >> (64 - bits);
        return (double(s_l) / (std::pow(2, bits)-1) * 2) - 1;
    }

    AudioSource* copy() {return new QuantizeFilter(*this);}
};

#endif
