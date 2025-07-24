#include "generator.hpp"
#include "builtin.hpp"
#include "../player/note.hpp"

#include <algorithm>
#include <cmath>

std::string Gen_Metadata::ToString() const {
    std::string ret = keyword;
    size_t maxKwdLen = Generator::GetLongestKeywordMeta().keyword.size();
    for(size_t i = 0; i < (maxKwdLen - keyword.size())/8 + 1; i++) {
        ret += '\t';
    }
    ret += (maxKwdLen == keyword.size()?"":"\tSyntax: ") + syntax + "\t" + desc;
    return ret;
}

void Generator::Init() {
    AddMetadata(Gen_Metadata("sine", SineGenerator::Create, "sine([freq_multiplier] [amplitude] [phase_offset])", "Generates a sine wave"));
    AddMetadata(Gen_Metadata("square", SquareGenerator::Create, "square([freq_multiplier] [amplitude] [phase_offset] [duty_cycle])", "Generates square wave"));
    AddMetadata(Gen_Metadata("triangle", TriangleGenerator::Create, "triangle([freq_multiplier] [amplitude] [phase_offset] [peak_point])", "Generatess arbitrary triangle wawe (yes, sawtooth too)"));
    AddMetadata(Gen_Metadata("noise", NoiseGenerator::Create, "noise([amplitude])", "Generates white noise"));
    AddMetadata(Gen_Metadata("register", Register::Create, "register { [src_1] [src_2] ... [src_n]}", "Wraps multiple sources into one"));
    AddMetadata(Gen_Metadata("sampled", SampledGenerator::Create, "sampled(<filename>)", "loads a (.wav) file from disk and plays it back. Note frequency controls playback speed")); //TODO: proper description
    AddMetadata(Gen_Metadata("none", NoGenerator::Create, "none", "Does nothing, plays silence"));
    AddMetadata(Gen_Metadata("constant", ValueGenerator::Create, "constant([value])", "Same as value, just for some backwards compatibility"));
    AddMetadata(Gen_Metadata("value", ValueGenerator::Create, "value([value])", "Returns a value interpolated over time"));
    default_meta = Gen_Metadata("value", ValueGenerator::CreateAsDefault, "value([value])", "Returns a value interpolated over time");
}

Generator::Generator(Interpolated<double> mul, Interpolated<double> gain, Interpolated<double> offs)
        : Source(), playing_notes(), m_phasemul(mul), m_gain(gain), m_phaseoffset(offs) {}

Generator::Generator(std::istream& str) : Source(), playing_notes() {
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

Generator::Generator(const Generator& g) : Source(g), playing_notes(), m_phasemul(g.m_phasemul), m_gain(g.m_gain), m_phaseoffset(g.m_phaseoffset) {
    playing_notes.reserve(g.playing_notes.size());
    for(const auto& note : g.playing_notes) { playing_notes.emplace_back(note->copy()); }
}

void Generator::addNote(std::unique_ptr<Note> note) {
    note->AddToSource(this);
    playing_notes.emplace_back(std::move(note));
}

double Generator::getSample(int srate) {
    for(size_t i = 0; i < playing_notes.size(); i++) {
        if(playing_notes[i]->IsComplete()) {
            playing_notes[i]->RemoveFromSource(this);
            playing_notes.erase(playing_notes.begin() + int(i));
            i--;
            continue;
        }
        playing_notes[i]->AddSample(this, srate);
    }
    return getAccumulator();
}

double Generator::getFrequencyMultiplier(double t) {
    return m_phasemul(t);
}

void Generator::operator()(double phase, double t, int, double note_amplitude) {
    m_accumulator += getSample(fmod(phase + m_phaseoffset(t), 1), t) * m_gain(t) * note_amplitude;
}

std::string Generator::getFormattedMetadata() {
    std::string ret = "";
    for(const Gen_Metadata& f : meta) {
        ret += f.keyword + "\t" + (f.keyword.size() < 8? "\t" : "") + f.syntax + "\t" + f.desc + "\n";
    }
    return ret;
}
void Generator::WriteBaseParams(std::ostream& str) const {
    m_phasemul.Write(str);
    str << "  ";
    m_gain.Write(str);
    str << "  ";
    m_phaseoffset.Write(str); 
}

void Generator::Write(std::ostream& str) const {
    std::string generator_name = ToString();
    std::transform(generator_name.begin(), generator_name.end(), generator_name.begin(), [](char c) -> char { return std::tolower(c); });
    str << generator_name << '(';
    WriteBaseParams(str);
    str << ") ";
}

Generator::~Generator() {}
