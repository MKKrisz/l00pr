#include <iostream>

#include "freq.hpp"


std::istream& operator>>(std::istream& str, Frequency& f) {
    f = Frequency(str);
    return str;
}

std::ostream& operator<<(std::ostream& str, const Frequency& f) { return str << f.getName(); }

template<>
Frequency logarithmicInterpolator <> (Frequency a, Frequency b, double t) {
    return pow(a.getFreq(), 1-t)*pow(b.getFreq(), t);
}

