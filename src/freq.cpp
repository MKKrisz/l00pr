#include <iostream>

#include "freq.hpp"


std::istream& operator>>(std::istream& str, Frequency& f) {
    str >> skipws;
    std::stringstream ss {""};
    if(!isdigit(str.peek())) {
        ss << char(str.get());
        if(isNoteMod(str.peek())) 
            ss << char(str.get());
    }
    int n;
    str >> n;
    ss << std::to_string(n);
    f = Frequency(ss);
    return str;
}

std::ostream& operator<<(std::ostream& str, const Frequency& f) { return str << f.getName(); }

template<>
Frequency logarithmicInterpolator <> (Frequency a, Frequency b, double t) {
    return pow(a.getFreq(), 1-t)*pow(b.getFreq(), t);
}

