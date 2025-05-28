#pragma once

#include "filter.hpp"

class DummyFilter : public Filter {
public:
    DummyFilter() : Filter(nullptr) {}
    DummyFilter(AudioSource* s) : Filter(s) {}
    DummyFilter(const DummyFilter& f) : Filter(f) {}
    DummyFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        if(str.peek() == '{'){
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }
    inline double filter(double sample, double, double, double) { return sample; }
    DummyFilter* copy() {return new DummyFilter(*this); }
    inline DummyFilter& operator=(const DummyFilter&) = default;
    std::string ToString() {return Filter::ToString() + "Dummy";}
    static DummyFilter* Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return new DummyFilter(str, srate, flags);
    }
};
