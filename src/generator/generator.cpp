#include "generator.hpp"
#include "builtin.hpp"

#include <cmath>

void Generator::Init() {
    AddMetadata(AS_Metadata("sine", SineGenerator::Create, "sine([freq_multiplier] [amplitude] [phase_offset])", ""));
    AddMetadata(AS_Metadata("square", SquareGenerator::Create, "square([freq_multiplier] [amplitude] [phase_offset] [duty_cycle])", ""));
    AddMetadata(AS_Metadata("triangle", TriangleGenerator::Create, "triangle([freq_multiplier] [amplitude] [phase_offset] [peak_point])", ""));
    AddMetadata(AS_Metadata("noise", NoiseGenerator::Create, "noise([freq_multiplier] [amplitude] [phase_offset])", "Only `amplitude` does anything"));
    AddMetadata(AS_Metadata("register", Register::Create, "register([fm] [amp] [po]){ [src_1] [src_2] ... [src_n]}", "`po` does nothing, (`src_k`: any source)"));
}

Generator::Generator(std::istream& str) : AudioSource() {
    Interpolated<double> a[3] = {1, 1, 0.0f};
    if((str >> skipws).peek() != '('){
        m_phasemul = a[0];
        m_gain = a[1];
        m_phaseoffset = a[2];
        shouldBeDefault = true;
        return;
    }
    shouldBeDefault = false;
    str.get();
    for(int i = 0; i < 3; i++) {
        if((str >> skipws).peek() == ')') {
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

void Generator::operator()(size_t noteId, double delta, double t, double, double extmul) {
#ifdef DEBUG
    if(noteId >= phases.size)
        throw std::out_of_range("Generator phases");
#endif
    if (getLengthBounds().has_value() && t > getLengthBounds().value().second) return;
    double& phase = phases[noteId];
    phases[noteId] = fmod(phase + delta * m_phasemul(t), 1);
    accumulator += (getSample(fmod(phase + m_phaseoffset(t), 1), t) * m_gain(t) * extmul);
}

std::string Generator::getFormattedMetadata() {
    std::string ret = "";
    for(const AS_Metadata& f : meta) {
        ret += f.keyword + "\t" + (f.keyword.size() < 8? "\t" : "") + f.syntax + "\t" + f.desc + "\n";
    }
    return ret;
}
