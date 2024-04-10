#ifndef L00PR_UTIL
#define L00PR_UTIL

#include <istream>
#include <ranges>
#include "concepts.hpp"

/// Reads in a frequency value from the input stream.
/// Format: 
///         <A-G>[b#x]<int> where 
///         <>: one of the specified characters must be present
///         []: one of the specified characters may be present
///         int: integer
/// Remarks:
///         If the first character is a digit instead of a capital letter
///         between A and G, interprets value as a float
/// Throws:
///         parse_error on failure
double getFreq(std::istream& stream);

/// Same as the function above except for c-strings.
/// @param n: number of characters read in
/// Remarks: this version of the function silently fails instead of throwing
///          an error
float getFreq(const char* s, int* n = nullptr);

/// Gets the line and column number of the current stream position in the most
/// inefficent way imaginable
/// Returns:
///         An std::pair containing the line number and the column number (in this order)
std::pair<int, int> getLineColNo(std::istream& stream);

/// Linear Interpolation for Arithmetic types
template<Arithmetic T>
inline T lerp(T a, T b, float t) {
    return (1-t)*a + t*b;
}

inline bool isNote(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'G');
}
inline bool isNoteMod(char c) {
    return (c == '#' || c == 'x' || c == 'b');
}

//loptam gtest_lite-ból
template <typename T>
bool almostEQ(T a, T  b) {
    // eps: ha a relatív, vagy abszolút hiba ettől kisebb, akkor elfogadjuk
    T eps = 10 * std::numeric_limits<T>::epsilon(); // 10-szer a legkisebb érték
    if (a == b) return true;
    if (fabs(a - b) < eps)
       return true;
    double aa = fabs(a);
    double ba = fabs(b);
    if (aa < ba) {
        aa = ba;
        ba = fabs(a);
    }
    return (aa - ba) < aa * eps;
}




//TODO: Refactor template arguments...
template <std::ranges::range U, std::floating_point F, typename T = std::ranges::range_value_t<U>::second_type>
    requires std::same_as<std::ranges::range_value_t<U>, std::pair<F, T>>
size_t bSearch(U data, F t) {
    if(data.size() == 0) return 0;
    size_t min = 0;
    size_t max = data.size()-1;
    size_t mid = (min + max)/2;
    while(min<=max && !almostEQ(data[mid].first, t)) {
        if(data[mid].first < t)
            min = mid+1;
        else
            max = mid-1;
        mid = (min + max)/2;
    }
    return mid;
}

template<std::ranges::range U, std::floating_point F, typename T = std::ranges::range_value_t<U>::second_type>
    requires std::same_as<std::ranges::range_value_t<U>, std::pair<F, T>>
void ordered_add(U& data, std::pair<F, T> p) {
    if(data.empty()) { 
        data.emplace_back(p);
        return;
    }
    size_t id = bSearch(data, p.first);
    if(data[id].first < p.first) id++;
    if(id == data.size())
        data.emplace_back(p);
    else
        data.insert(std::next(data.begin(), id), p);
}



std::istream& skipws(std::istream&);


#endif
