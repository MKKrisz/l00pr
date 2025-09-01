#pragma once

#include "filter.hpp"
#include <algorithm>

class ClampFilter : public Filter{
    double lower = -std::numeric_limits<double>::infinity();
    double upper = std::numeric_limits<double>::infinity();
public:
    ClampFilter() : Filter(nullptr) {}
    ClampFilter(Source* s) : Filter(s) {}
    ClampFilter(const ClampFilter& f) : Filter(f), lower(f.lower), upper(f.upper) {}
    ClampFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> skipws;
        if (isdigit(str.peek())) {str >> lower;}
        str >> expect(':') >> skipws;
        if (isdigit(str.peek())) {str >> upper;}
        str >> expect(')') >> skipws;
        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
    }

    inline double filter(double sample, double, double, int) { return std::clamp(sample, lower, upper); }

    std::unique_ptr<Source> copy() {return std::make_unique<ClampFilter>(*this); }

    inline ClampFilter& operator=(const ClampFilter&) = default;

    std::string ToString() const {return Filter::ToString() + "Clamp(" + std::to_string(lower) + ":" + std::to_string(upper) + ")";}
    std::string GetNameAndParams() const { return "bounds(" + std::to_string(lower) + ":" + std::to_string(upper) + ")"; }

    static std::unique_ptr<ClampFilter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<ClampFilter>(str, srate, flags);
    }
};
