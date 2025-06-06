#ifndef L00PR_RANGE
#define L00PR_RANGE

#include <vector>
#include <stdexcept>
#include "concepts.hpp"
#include "interpolator.hpp"
#include "interpolated.hpp"
#include "util.hpp"
#include "exceptions/parse_error.hpp"

/// <summary> 
/// Data structure to parse, store and interpolate between values with no specific time value 
/// Mostly used for random note generation to define the ranges of values to use.
/// <summary>
/// <seealso cref="Interpolated"/>
/// <seealso cref="RandomNote"/>
template <typename T>
class Range {

    /// <summary> Internal data structure. </summary>
    std::vector<T> data;

    /// <summary> Boolean that indicates wether the return value of get should be interpolated or a fix value. </summary>
    bool fixValue = true;
public:
    /// <summary> Adds a value to this Range. </summary>
    inline void add(const T& t) { data.emplace_back(t); }

    /// <summary> Sets the fixvalue flag. </summary>
    inline void setFix(bool t) { fixValue = t; }
    /// <summary> Gets the fixvalue flag. </summary>
    inline bool isFix() { return fixValue; }

    /// <summary> Returns the size of the internal data structure. </summary>
    inline size_t size() { return data.size();}

    /// <summary> Returns 
    inline T get(double val) const {
        if(data.empty())
            throw std::out_of_range("Empty Range");
        if(val < 0 || val > 1) 
            throw std::out_of_range("Range float-based getter");
        if(fixValue) {
            return data[size_t(val*data.size())];
        }
        if(data.size() < 2)
            throw std::out_of_range("Insufficent size of Range");

        if constexpr (is_interpolated<T>::value) {
            return linearInterpolator(data[0][0].second, data[1][0].second, val);
        }
        else 
            return linearInterpolator(data[0], data[1], val);
    }

    // Constructors
    Range() : data() {}
    Range(const T& data) : data() {this->data.emplace_back(data);}
    Range(const T& low, const T& high) : data(), fixValue(false) {
        data.emplace_back(low);
        data.emplace_back(high);
    }
    template <std::ranges::range U> 
        requires std::same_as<std::ranges::range_value_t<U>, T>
    Range(const U& p) : data() {
        for(auto i = p.begin(); i != p.end(); i++) {
            data.emplace_back(*i);
        }
    }

    /// <summary> Returns a reference to the value in internal structure at index 'i' </summary>
    T& operator[](size_t i) { 
        if(i >= data.size())
            throw std::out_of_range("Range");
        return data[i];
    }
};

/// <summary> Parser for Ranges. Range syntax can be: <value>; (<value1> -- <value2>); (<value1> <value2> ... <value_n>) </summary>
template<typename T>
std::istream& operator>>(std::istream& str, Range<T>& range) {
    T val;
    str >> skipws;
    if(str.peek() != '(') {
        str >> val;
        range.add(val);
        return str;
    }
    str.get();
    str >> skipws >> val >> skipws;
    range.add(val);
    val = T();
    if(str.peek() == '-') {
        str.get();
        str >> skipws >> val >> expect(')');
        range.setFix(false);
        range.add(val);
        return str;
    }
    while(str.good() && str.peek() != ')') {
        str >> skipws >> val >> skipws;
        range.add(val);
        val = T();
    }
    if(!str.good())
        throw parse_error(str, "Unexpected parse error, potencially missing ')'");
    str.get();
    return str;
}

#endif
