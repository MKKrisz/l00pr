#ifndef L00PR_RANGE
#define L00PR_RANGE

#include <vector>
#include <stdexcept>
#include "concepts.hpp"
#include "interpolator.hpp"
#include "interpolated.hpp"
#include "util.hpp"
#include "exceptions/parse_error.hpp"

template <typename T>
class Range {
    std::vector<T> data;
    bool fixValue = true;
public:
    inline void add(T& t) { data.emplace_back(t); }
    inline void setFix(bool t) { fixValue = t; }
    inline bool isFix() { return fixValue; }
    inline size_t size() { return data.size();}
    inline T get(double val) {
        if(val < 0 || val > 1) 
            throw std::out_of_range("Range float-based getter");
        if(fixValue) {
            return data[size_t(val*data.size())];
        }
        if constexpr (is_interpolated<T>::value) {
            return linearInterpolator(data[0][0].second, data[1][0].second, val);
        }
        else 
            return linearInterpolator(data[0], data[1], val);
    }

    Range() : data() {}
    Range(const T& data) : data() {this->data.emplace_back(data);}
    Range(const T& low, const T& high) : data(), fixValue(false) {
        data.emplace_back(low);
        data.emplace_back(high);
    }
    template <std::ranges::range U> 
        requires std::same_as<std::ranges::range_value_t<U>, T>
    Range(U& p) : data() {
        for(auto i = p.begin(); i != p.end(); i++) {
            data.emplace_back(*i);
        }
    }

    T& operator[](size_t i) { 
        if(i >= data.size())
            throw std::out_of_range("Range");
        return data[i];
    }
};

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
        str >> skipws >> val >> skipws;
        range.setFix(false);
        range.add(val);
        if(str.peek() != ')')
            throw parse_error(str, "Expected ')'");
        str.get();
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
