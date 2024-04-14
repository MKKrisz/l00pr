#include "interpolated.hpp"
#include "util.hpp"
#include "exceptions/parse_error.hpp"


template<>
float Interpolated<float>::basic_interpreter(const char* s, int* n) {
    return getFreq(s, n);
}

