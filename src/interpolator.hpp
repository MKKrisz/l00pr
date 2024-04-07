#ifndef L00PR_INTERPOLATOR
#define L00PR_INTERPOLATOR

#include <cmath>

template <typename T>
T noInterpolator(T a, T b, float t) { return a; }

template <typename T>
T reverseInterpolator(T a, T b, float t) { return b; }

template <typename T>
T linearInterpolator(T a, T b, float t) { return (1-t)*a + t*b; }

template <typename T>
T logarithmicInterpolator(T a, T b, float t) { return pow(a, 1-t)*pow(b, t);}



#endif
