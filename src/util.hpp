#ifndef L00PR_UTIL
#define L00PR_UTIL

#include <istream>

#include "generator/generator.hpp"
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


#endif
