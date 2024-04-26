#ifndef L00PR_INTERPOLATOR
#define L00PR_INTERPOLATOR

#include <cmath>

/// <summary>
/// Rudimentary interpolator that returns the first argument
/// </summary>
template <typename T, std::floating_point F = double>
T noInterpolator(T a, T b, F t) { return a; }

/// <summary>
/// Rudimentary interpolator that returns the second argument
/// </summary>
template <typename T, std::floating_point F = double>
T reverseInterpolator(T a, T b, F t) { return b; }

/// <summary>
/// Linear interpolator. (see: https://en.wikipedia.org/wiki/Linear_interpolation)
/// </summary>
/// <param name='a'> Return value when t is 0. </param> 
/// <param name='b'> Return value when t is 1. </param> 
/// <param name='t'> A value between 0 and 1 that represents where we are between a and b</param>
template <typename T, std::floating_point F = double>
T linearInterpolator(T a, T b, F t) { return (1-t)*a + t*b; }

/// <summary>
/// Logarithmic interpolator. Useful when trying to achieve a linear effect while interpolating amplitudes and frequencies
/// </summary>
/// <param name='a'> Return value when t is 0. </param> 
/// <param name='b'> Return value when t is 1. </param> 
/// <param name='t'> A value between 0 and 1 that represents where we are between a and b</param>
template <typename T, std::floating_point F = double>
T logarithmicInterpolator(T a, T b, F t) { return pow(a, 1-t)*pow(b, t);}



#endif
