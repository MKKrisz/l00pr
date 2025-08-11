#pragma once
#include "filter.hpp"

class AbsoluteValue : public Filter {
public:
    AbsoluteValue() : Filter(nullptr) {}
    AbsoluteValue(Source* s) : Filter(s) {}
    AbsoluteValue(const AbsoluteValue& f) : Filter(f) {}
    AbsoluteValue(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
    }
    inline double filter(double sample, double, double, int) { return fabs(sample); }
    std::unique_ptr<Source> copy() {return std::make_unique<AbsoluteValue>(*this); }
    inline AbsoluteValue& operator=(const AbsoluteValue&) = default;
    std::string ToString() const {return Filter::ToString() + "AbsoluteValue";}
    std::string GetNameAndParams() const { return "absolute"; }
    static std::unique_ptr<AbsoluteValue> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<AbsoluteValue>(str, srate, flags);
    }
};
