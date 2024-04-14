#ifndef L00PR_INTERPOLATOR
#define L00PR_INTERPOLATOR

#include <cmath>


template <typename T, std::floating_point F = double>
T noInterpolator(T a, T b, F t) { return a; }

template <typename T, std::floating_point F = double>
T reverseInterpolator(T a, T b, F t) { return b; }

template <typename T, std::floating_point F = double>
T linearInterpolator(T a, T b, F t) { return (1-t)*a + t*b; }

template <typename T, std::floating_point F = double>
T logarithmicInterpolator(T a, T b, F t) { return pow(a, 1-t)*pow(b, t);}



#endif
