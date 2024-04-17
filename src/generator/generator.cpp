#include "generator.hpp"
#include "builtin.hpp"

#include <cmath>

double Generator::operator()(int noteId, double delta, double t) {
    double& phase = phases[noteId];
    phases[noteId] = fmod(phase + delta * m_phasemul(t), 1);
    return getSample(fmod(phase + m_phaseoffset(t), 1), t) * m_gain(t);
}

std::istream& operator>> (std::istream& stream, Generator** gen) {
    stream >> skipws;
    std::string buf;
    bool parsed = false;
    for(size_t i = 0; i < std::string("triangle").size(); i++) {
        buf += tolower(stream.get());
        if(buf == "sine")     { *gen = new SineGenerator(stream); parsed = true; break;}
        if(buf == "square")   { *gen = new SquareGenerator(stream); parsed = true; break; }
        if(buf == "triangle") { *gen = new TriangleGenerator(stream); parsed = true; break; }
        if(buf == "register") { *gen = new Register(stream); parsed = true; break; }
        if(buf == "noise")    { *gen = new NoiseGenerator(stream); parsed = true; break; }
    }
    if(!parsed)
        throw parse_error(stream, "Generator type \"" + buf + "\" does not exist.");
    return stream;
}
