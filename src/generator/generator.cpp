#include "generator.hpp"
#include "builtin.hpp"

#include <cmath>

Generator::Generator(std::istream& str) : AudioSource() {
    if((str >> skipws).peek() != '('){ 
        shouldBeDefault = true;
        return;
    }
    shouldBeDefault = false;
    str.get();
    Interpolated<double> a[3] = {1, 1, 0.0f};
    for(int i = 0; i < 3; i++) {
        if((str >> skipws).peek() == ')') {
            shouldBeDefault = true;
            break;
        }
        a[i].Clear();
        str >> a[i];
    }
    if((str >> skipws).peek() == ')') {
        shouldBeDefault = true;
        str.get();
    }
    m_phasemul = a[0];
    m_gain = a[1];
    m_phaseoffset = a[2];

}

double Generator::operator()(int noteId, double delta, double t, double) {
    double& phase = phases[noteId];
    phases[noteId] = fmod(phase + delta * m_phasemul(t), 1);
    return getSample(fmod(phase + m_phaseoffset(t), 1), t) * m_gain(t);
}

const std::vector<std::string> genNames = {"sine", "square", "triangle", "noise", "register"};

bool Generator::ValidName(std::string& str) {
    for(const std::string& g : genNames) {
        if(str == g) return true;
    }
    return false;
}

size_t Generator::LongestName() {
    size_t l = 0;
    for(const std::string& gn : genNames) {
        if(l < gn.size()) l = gn.size();
    }
    return l;
}

Generator* Generator::Make(std::string& name, std::istream& str, int srate, MakeFlags& flags) {
    if(name == "sine")     return new SineGenerator(str);
    if(name == "square")   return new SquareGenerator(str);
    if(name == "triangle") return new TriangleGenerator(str);
    if(name == "noise")    return new NoiseGenerator(str);
    if(name == "register") return new Register(str, srate, flags);
    throw parse_error(str, "No generator with name \"" + name + "\" exists");
}
