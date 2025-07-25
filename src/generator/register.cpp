#include "register.hpp"
#include "../audiosource.hpp"
#include "../player/note.hpp"

Register::Register(std::vector<Source*> gen) {
    generators.reserve(gen.size());
    for(auto g : gen) {
        generators.emplace_back(g->copy());
    }
}
Register::Register(const Register& r) : Generator(r) {
    generators.reserve(r.generators.size());
    for(auto& g : r.generators) {
        generators.emplace_back(g->copy());
    }
}

void Register::operator()(double p, double t, int srate, double a) {
    for(auto& g : generators) {
        (*g)(p, t, srate, a);
    }
}

double Register::getSample(int srate) {
    for(auto& g : generators) {
        m_accumulator += g->getSample(srate);
    }
    return getAccumulator();
}
double Register::getSingleSample(double p, double t, int s) {
    double sum = 0;
    for(auto& g : generators) {
        sum += g->getSingleSample(p, t, s);
    }
    return sum;
}

void Register::addNote(std::unique_ptr<Note> note) {
    for(auto& g : generators) {
        g->addNote(note->copy());
    }
}

Register::Register(std::istream& stream, const int srate, const MakeFlags& flags) : Generator() {
    stream >> expect('{');
    while((stream >> skipws).peek() != '}') {
        generators.emplace_back(std::move(Source::Make(stream, srate, flags)));
    }
    stream.get();
}

std::unique_ptr<Source> Register::copy() {
    return std::make_unique<Register>(*this);
}


std::unique_ptr<Register> Register::Create(std::istream& str, const int srate, const MakeFlags& flags) {
    return std::make_unique<Register>(str, srate, flags);
}
void Register::Write(std::ostream& str) const {
    str << "register(";
    Generator::WriteBaseParams(str);
    str << ") {";
    for(auto& g : generators) {
        str << std::endl;
        g->Write(str);
    }
    str << '}';
}
std::vector<SourceRef*> Register::getSourceRefs() {
    std::vector<SourceRef*> ret {};
    for(auto& g : generators) {
        auto g_refs = g->getSourceRefs();
        ret.insert(ret.end(), g_refs.begin(), g_refs.end());
    }
    return ret;
}
std::vector<Source*> Register::getLabeled() {
    std::vector<Source*> labels {};
    for(auto& g : generators) {
        auto g_labels = g->getLabeled();
        labels.insert(labels.end(), g_labels.begin(), g_labels.end());
    }
    auto self = Source::getLabeled();
    labels.insert(labels.end(), self.begin(), self.end());
    return labels;
}
