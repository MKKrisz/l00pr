#include "register.hpp"
#include "../audiosource.hpp"

Register::Register(std::vector<AudioSource*> gen) {
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

void Register::operator()(size_t noteId, double d, double t, double srate, double extmul) {
    for(auto& g : generators) {
        (*g)(noteId, d * m_phasemul(t), t, srate, extmul);
    }
}

double Register::calc() {
    double sum = feedback;
    for(auto& g : generators) {
        sum += g->calc();
    }
    return sum;
}

void Register::addPhase() {
    for(auto& g : generators) {
        g->addPhase();
    }
}

void Register::removePhase(int id) {
    for(auto& g : generators) {
        g->removePhase(id);
    }
}

Register::Register(std::istream& stream, const int srate, const MakeFlags& flags) : Generator(stream) {
    stream >> expect('{');
    while((stream >> skipws).peek() != '}') {
        generators.emplace_back(std::move(AudioSource::Make(stream, srate, flags)));
    }
    stream.get();
}

std::unique_ptr<AudioSource> Register::copy() {
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
    AudioSource::WriteLengthBounds(str);
}
