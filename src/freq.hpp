#ifndef L00PR_FREQ
#define L00PR_FREQ

#include <string>
#include <sstream>
#include "util.hpp"

class Frequency {
    std::string name;
    double val;
public:
    inline constexpr double getFreq() const noexcept {return val;}
    inline constexpr std::string getName() const noexcept {return name;}

    inline Frequency(std::string& name) : name(name), val(0) {
        std::stringstream ss(name);
        val = ::getFreq(ss);
    }
    inline Frequency(std::stringstream& name) : name(name.str()), val(::getFreq(name)) {}
    inline Frequency(double val) : name(std::to_string(val)), val(val) {}

    inline Frequency operator+(Frequency& f) { return Frequency(val + f.val); }
    inline Frequency operator-(Frequency& f) { return Frequency(val - f.val); }
    inline Frequency operator*(Frequency& f) { return Frequency(val * f.val); }
    inline Frequency operator/(Frequency& f) { return Frequency(val / f.val); }
};

std::istream& operator>>(std::istream& str, Frequency& f);
std::ostream& operator<<(std::ostream& str, Frequency& f);
#endif
