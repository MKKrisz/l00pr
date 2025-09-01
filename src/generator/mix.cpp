#include "mix.hpp"
#include "../audiosource.hpp"
#include "../player/note.hpp"

Mixer::Mixer(std::vector<Source*> gen) {
    generators.reserve(gen.size());
    for(auto g : gen) {
        generators.emplace_back(g->copy());
    }
}
Mixer::Mixer(const Mixer& r) : Generator(r) {
    generators.reserve(r.generators.size());
    for(auto& g : r.generators) {
        generators.emplace_back(g->copy());
    }
}

void Mixer::operator()(double p, double t, int srate, double a) {
    actual_amplitude = 0;
    for(auto& g : generators) {
        m_accumulator = 0;
        (*g)(p, t, srate, a);
        actual_amplitude *= m_accumulator;
    }
    m_accumulator = actual_amplitude;
}

double Mixer::getSample(int srate) {
    for(auto& g : generators) {
        m_accumulator *= g->getSample(srate);
    }
    return getAccumulator();
}
double Mixer::getSingleSample(double p, double t, int s) {
    double sum = 0;
    for(auto& g : generators) {
        sum *= g->getSingleSample(p, t, s);
    }
    return sum;
}

void Mixer::addNote(std::unique_ptr<Note> note) {
    for(auto& g : generators) {
        g->addNote(note->copy());
    }
}

Mixer::Mixer(std::istream& stream, const int srate, const MakeFlags& flags) : Generator() {
    stream >> expect('{');
    while((stream >> skipws).peek() != '}') {
        generators.emplace_back(std::move(Source::Make(stream, srate, flags)));
    }
    stream.get();
}

std::unique_ptr<Source> Mixer::copy() {
    return std::make_unique<Mixer>(*this);
}


std::unique_ptr<Mixer> Mixer::Create(std::istream& str, const int srate, const MakeFlags& flags) {
    return std::make_unique<Mixer>(str, srate, flags);
}
void Mixer::Write(std::ostream& str) const {
    str << "mixer {";
    for(auto& g : generators) {
        str << std::endl;
        g->Write(str);
    }
    str << '}';
}
std::vector<SourceRef*> Mixer::getSourceRefs() {
    std::vector<SourceRef*> ret {};
    for(auto& g : generators) {
        auto g_refs = g->getSourceRefs();
        ret.insert(ret.end(), g_refs.begin(), g_refs.end());
    }
    return ret;
}
std::vector<Source*> Mixer::getLabeled() {
    std::vector<Source*> labels {};
    for(auto& g : generators) {
        auto g_labels = g->getLabeled();
        labels.insert(labels.end(), g_labels.begin(), g_labels.end());
    }
    auto self = Source::getLabeled();
    labels.insert(labels.end(), self.begin(), self.end());
    return labels;
}
