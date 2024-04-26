#ifndef LOOPR_CONCEPTS
#define LOOPR_CONCEPTS

#include <concepts>

/// <summary> 
/// Any type that can be multiplied, divided by, added to, subtracted from a float
/// </summary>
template <typename T>
concept Arithmetic = requires(T a, double b) {
    a * b;
    a / b;
    a + b;
    a - b;
};

#endif
