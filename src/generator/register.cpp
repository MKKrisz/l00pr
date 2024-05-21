#include "register.hpp"
#include "../audiosource.hpp"

Register::Register(std::vector<AudioSource*> gen) : generators(gen) {}

void Register::operator()(size_t noteId, double d, double t, double srate, double extmul) {
    for(AudioSource* g : generators) {
        (*g)(noteId, d * m_phasemul(t), t, srate, extmul);
    }
}

double Register::calc() {
    double sum = feedback;
    for(AudioSource* g : generators) {
        sum += g->calc();
    }
    return sum;
}

void Register::addPhase() {
    for(AudioSource* g : generators) {
        g->addPhase();
    }
}

void Register::removePhase(int id) {
    for(AudioSource* g : generators) {
        g->removePhase(id);
    }
}

Register::~Register() {
    for(AudioSource* g : generators) {
        delete g;
    }
}

Register::Register(std::istream& stream, const int srate, const MakeFlags& flags) : Generator(stream) {
    stream >> expect('{');
    while((stream >> skipws).peek() != '}') {
        AudioSource* src = AudioSource::Make(stream, srate, flags);
        generators.push_back(src);
    }
    stream.get();
}

Register* Register::copy() {
    std::vector<AudioSource*> gs;
    for(AudioSource* g : generators) {
        gs.emplace_back(g->copy());
    }
    return new Register(gs);
}


Register* Register::Create(std::istream& str, const int srate, const MakeFlags& flags) {
    return new Register(str, srate, flags);
}
