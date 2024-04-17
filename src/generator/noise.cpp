#include "noise.hpp"

NoiseGenerator::NoiseGenerator(std::istream& stream) : NoiseGenerator() {
    if((stream >> std::ws).peek() != '(') return;
    stream.get();
    Interpolated<double> a[3] = {1, 1, 0.0f};
    for(int i = 0; i < 3; i ++) {
        if((stream >> std::ws).peek() == ')') break;
        a[i].Clear();
        stream >> a[i];
    }
    if((stream >> std::ws).peek() != ')') 
        throw parse_error(stream, "Excepted ')'");
    stream.get();
    m_gain = a[1];
    m_phasemul = a[0];
    m_phaseoffset = a[2];

}
