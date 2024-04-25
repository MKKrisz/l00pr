#ifndef L00PR_FREQ
#define L00PR_FREQ

#include <string>
#include <sstream>
#include "util.hpp"
#include "interpolator.hpp"

class Frequency {
    std::string name = "";
    double val = 0;
public:
    inline constexpr double getFreq() const noexcept {return val;}
    inline constexpr std::string getName() const noexcept {
        if(name != "")
            return name;
        return std::to_string(val);
    }

    inline Frequency() {}
    inline Frequency(std::string& name) : name(name), val(0) {
        std::stringstream ss(name);
        val = ::getFreq(ss);
    }
    inline Frequency(std::stringstream& name) : name(name.str()), val(::getFreq(name)) {}
    inline Frequency(double val) noexcept : name(""), val(val) {}

    inline Frequency operator+(Frequency f) const { return Frequency(val + f.val); }
    inline Frequency operator-(Frequency f) const { return Frequency(val - f.val); }
    inline Frequency operator*(Frequency f) const { return Frequency(val * f.val); }
    inline Frequency operator/(Frequency f) const { return Frequency(val / f.val); }

    inline Frequency operator+(double f) const { return Frequency(val + f); }
    inline Frequency operator-(double f) const { return Frequency(val - f); }
    inline Frequency operator*(double f) const { return Frequency(val * f); }
    inline Frequency operator/(double f) const { return Frequency(val / f); }

};


inline Frequency operator*(double f, Frequency& fr) { return Frequency(fr.getFreq() * f); }


std::istream& operator>>(std::istream& str, Frequency& f);
std::ostream& operator<<(std::ostream& str, const Frequency& f);

template<>
Frequency logarithmicInterpolator <> (Frequency a, Frequency b, double t);

#endif
