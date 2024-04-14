#include "interpolated.hpp"
#include "util.hpp"
#include "exceptions/parse_error.hpp"


template<>
float Interpolated<float>::basic_interpreter(const char* s, int* n) {
    return getFreq(s, n);
}

template<std::floating_point F>
std::istream& operator>> (std::istream& stream, Interpolated<F>& p) {
    stream >> std::ws;
    size_t start = stream.tellg();
    int i = 0;
    while(true) {
        double t;
        double val;
        if(isdigit((stream >> std::ws).peek())) {
            stream >> t;
            if ((stream >> std::ws).peek() != ':') {
                if (i == 0) {
                    stream.seekg(start);
                    val = getFreq(stream);
                    p.data.emplace_back(std::make_pair(0, val));
                    break;
                } else throw parse_error(stream, "Couldn't interpret interpolated value: No ':' after timestamp value");
            }
        }
        else {
            val = getFreq(stream);
            p.data.emplace_back(std::make_pair(0, val));
            break;
        }
        stream.get();
        val = getFreq(stream);
        p.data.emplace_back(std::make_pair(t, val));
        if((stream >> std::ws).peek() != '-') break;
        i++;
        stream.get();
    }
    p.sort();
    return stream;

}

