#include "generator.hpp"
#include "builtin.hpp"

#include <cmath>

double Generator::operator()(int noteId, double delta, double t) {
    double& phase = phases[noteId];
    phases[noteId] = fmod(phase + delta * m_phasemul(t), 1);
    return getSample(fmod(phase + m_phaseoffset(t), 1), t) * m_gain(t);
}

std::istream& operator>> (std::istream& stream, Generator** gen) {
    std::string buf;
    for(int i = 0; i < std::string("triangle").size(); i++) {
        buf += tolower(stream.get());
        if(buf == "sine")     { *gen = new SineGenerator(stream); break;}
        if(buf == "square")   { *gen = new SquareGenerator(stream); break; }
        if(buf == "triangle") { *gen = new TriangleGenerator(stream); break; }
        if(buf == "register") { *gen = new Register(stream); break; }
    }
    return stream;
}
