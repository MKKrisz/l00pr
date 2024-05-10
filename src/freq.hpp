#ifndef L00PR_FREQ
#define L00PR_FREQ

#include <string>
#include <sstream>
#include "util.hpp"
#include "interpolator.hpp"

/// <summary>
/// Class to parse frequency (and also piano key name) values into 
/// Syntax: <just_a_float> or <A-G>[#, b, x]<octave>
/// </summary>
class Frequency {
    /// <summary> The string that got parsed when constructing this object </summary>
    std::string name = "";

    /// <summary> The actual frequency representation of this object </summary>
    double val = 0;
public:
    /// <summary> Returns this object's value </summary>
    inline constexpr double getFreq() const noexcept {return val;}

    /// <summary> Returns the string representation of this object </summary>
    inline constexpr std::string getName() const noexcept {
        if(name != "")
            return name;
        return std::to_string(val);
    }

    // Constructors
    inline Frequency() {}
    inline Frequency(const std::string& name) : name(name), val(0) {
        std::stringstream ss(name);
        val = ::getFreq(ss);
    }
    inline Frequency(std::istream& str) : name(), val() {
        str >> skipws;

        int start = str.tellg();
        val = ::getFreq(str);

        str.clear();
        int end = str.tellg();

        str.seekg(start);
        std::string s = "";
        char c;
        while(str.tellg() != end && str.get(c)) {s += c;}

        name = s;
    }
    inline Frequency(double val) noexcept : name(""), val(val) {}

    // Operators to support interpolation
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

/// <summary> Parses a frequency value from 'str'. </summary>
std::istream& operator>>(std::istream& str, Frequency& f);

/// <summary> Outputs a frequency value (preferably it's name member) to 'str'. </summary>
std::ostream& operator<<(std::ostream& str, const Frequency& f);

/// <summary> Overloaded interpolator for frequencies </summary>
template<>
Frequency logarithmicInterpolator <> (Frequency a, Frequency b, double t);

#endif
