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
    AddMetadata(Gen_Metadata("constant", ValueGenerator::Create, "constant([value])", "Same as value, just for some backwards compatibility"));
    AddMetadata(Gen_Metadata("noise", NoiseGenerator::Create, "noise([amplitude])", "Generates white noise"));
    AddMetadata(Gen_Metadata("none", NoGenerator::Create, "none", "Does nothing, plays silence"));
    AddMetadata(Gen_Metadata("register", Register::Create, "register { [src_1] [src_2] ... [src_n]}", "Wraps multiple sources into one"));
    AddMetadata(Gen_Metadata("sampled", SampledGenerator::Create, "sampled(<filename>)", "loads a (.wav) file from disk and plays it back. Note frequency controls playback speed")); //TODO: proper description
    AddMetadata(Gen_Metadata("sine", SineGenerator::Create, "sine([freq_multiplier] [amplitude] [phase_offset])", "Generates a sine wave"));
    AddMetadata(Gen_Metadata("square", SquareGenerator::Create, "square([freq_multiplier] [amplitude] [phase_offset] [duty_cycle])", "Generates square wave"));
    AddMetadata(Gen_Metadata("triangle", TriangleGenerator::Create, "triangle([freq_multiplier] [amplitude] [phase_offset] [peak_point])", "Generatess arbitrary triangle wawe (yes, sawtooth too)"));
    AddMetadata(Gen_Metadata("value", ValueGenerator::Create, "value([value])", "Returns a value interpolated over time"));
    default_meta = Gen_Metadata("value", ValueGenerator::CreateAsDefault, "value([value])", "Returns a value interpolated over time");
}

Generator::Generator(bool will_self_initialize) : Source(), playing_notes() {if(!will_self_initialize) {throw std::runtime_error("Generator(bool): will_self_initialize was false"); }}
Generator::Generator(Interpolated<double> mul, Interpolated<double> gain, Interpolated<double> offs)
        : Source(), playing_notes(), m_phasemul(std::make_unique<ValueGenerator>(mul)), m_gain(std::make_unique<ValueGenerator>(gain)), m_phaseoffset(std::make_unique<ValueGenerator>(offs)) {}

Generator::Generator(std::istream& str, int samplerate) : Source(), playing_notes() {
    Interpolated<double> a[3] = {1.0, 1.0, 0.0};
    m_phasemul = std::make_unique<ValueGenerator>(a[0]);
    m_gain = std::make_unique<ValueGenerator>(a[1]);
    m_phaseoffset = std::make_unique<ValueGenerator>(a[2]);

    if((str >> skipws).peek() != '('){
        shouldBeDefault = true;
        return;
    }
    shouldBeDefault = false;
    str.get();
    if((str >> skipws).peek() == ')') {
        return;
    }
    m_phasemul = Source::Make(str, samplerate, MakeFlags::all);
    if((str >> skipws).peek() == ')') {
        return;
    }
    m_gain = Source::Make(str, samplerate, MakeFlags::all);
    if((str >> skipws).peek() == ')') {
        return;
    }
    m_phaseoffset = Source::Make(str, samplerate, MakeFlags::all);
    if((str >> skipws).peek() == ')') {
        shouldBeDefault = true;
        str.get();
    }
}

Generator::Generator(const Generator& g) : Source(g), playing_notes(), m_phasemul(g.m_phasemul->copy()), m_gain(g.m_gain->copy()), m_phaseoffset(g.m_phaseoffset->copy()) {
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

double Generator::getSingleSample(double phase, double t, int srate) {
    const double fm = getFrequencyMultiplier(t, srate);
    return getSample(fmod(phase*fm + m_phaseoffset->getSingleSample(t, t, srate), 1), t, srate) * m_gain->getSingleSample(t, t, srate);
}

double Generator::getFrequencyMultiplier(double t, int srate) {
    const double fm = m_phasemul->getFrequencyMultiplier(t, srate);
    (*m_phasemul)(t*fm, t, srate, 1);
    return m_phasemul->getSample(srate);
}

void Generator::operator()(double phase, double t, int srate, double note_amplitude) {
    phase += m_phaseoffset->getSingleSample(t, t, srate);
    note_amplitude *= m_gain->getSingleSample(t, t, srate);
    m_accumulator += getSample(fmod(phase, 1), t, srate) * note_amplitude;
}

std::string Generator::getFormattedMetadata() {
    std::string ret = "";
    for(const Gen_Metadata& f : meta) {
        ret += f.keyword + "\t" + (f.keyword.size() < 8? "\t" : "") + f.syntax + "\t" + f.desc + "\n";
    }
    return ret;
}
void Generator::WriteBaseParams(std::ostream& str) const {
    m_phasemul->Write(str);
    str << "  ";
    m_gain->Write(str);
    str << "  ";
    m_phaseoffset->Write(str); 
}

void Generator::Write(std::ostream& str) const {
    std::string generator_name = ToString();
    std::transform(generator_name.begin(), generator_name.end(), generator_name.begin(), [](char c) -> char { return std::tolower(c); });
    str << generator_name << '(';
    WriteBaseParams(str);
    str << ") ";
}

Generator::~Generator() {}
