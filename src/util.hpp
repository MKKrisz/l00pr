#ifndef L00PR_UTIL
#define L00PR_UTIL

#include <istream>
#include <ranges>
#include <cmath>
#include "concepts.hpp"

/// <summary> 
/// Reads in a frequency value from the input stream.
/// Format: 
///         <A-G>[b#x]<int> where 
///         <>: one of the specified characters must be present
///         []: one of the specified characters may be present
///         int: integer
/// </summary>
/// <remarks>
///         If the first character is a digit instead of a capital letter
///         between A and G, interprets value as a float
/// </remarks>
/// <exception cref="parse_error"> On failure. </exception>
double getFreq(std::istream& stream);

/// <summary>
/// Gets the line and column number of the current stream position in the most
/// inefficent way imaginable
/// </summary>
/// <returns> An std::pair containing the line number and the column number (in this order) </returns>
std::pair<int, int> getLineColNo(std::istream& stream);

/// Linear Interpolation for Arithmetic types
template<Arithmetic T>
inline T lerp(T a, T b, float t) {
    return (1-t)*a + t*b;
}

inline bool isNote(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'G') || c == '-';
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

/// <summary> Binary search algorithm for arbitrary ordered ranges of std::pair<F, T>, where F is a floating point type </summary>
template <std::ranges::range U, std::floating_point F, typename T = std::ranges::range_value_t<U>::second_type>
    requires std::same_as<std::ranges::range_value_t<U>, std::pair<F, T>>
size_t bSearch(U data, F t) {
    if(data.size() == 0) return 0;
    int min = 0;
    int max = data.size()-1;
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

/// <summary> Ordered insertion for arbitrary ranges of std::pair<F, T> where F is a floating point type </summary>
template<std::ranges::range U, std::floating_point F, typename T = std::ranges::range_value_t<U>::second_type>
    requires std::same_as<std::ranges::range_value_t<U>, std::pair<F, T>>
void ordered_add(U& data, std::pair<F, T> p) {
    if(data.empty()) { 
        data.emplace_back(p);
        return;
    }
    size_t id = bSearch(data, p.first) + 1;
    if(id == data.size())
        data.emplace_back(p);
    else
        data.insert(std::next(data.begin(), id), p);
}

/// <summary> Stream manipulator that skips whitespace and comments </summary>
std::istream& skipws(std::istream&);

/// <summary> expect() stream manipulator helper struct </summary>
struct exp_p {
    char val;
};

/// <summary> Stream manipulator that throws an exception if the next non-whitespace character is not 'c' </summary>
exp_p expect(char c);
std::istream& operator>>(std::istream&, const exp_p&);


#endif
