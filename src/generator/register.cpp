#include "register.hpp"
#include "../audiosource.hpp"

Register::Register(std::vector<AudioSource*> gen) : generators(gen) {}

double Register::operator()(int noteId, double d, double t, double srate) {
    double sum = 0;
    for(AudioSource* g : generators) {
        sum += (*g)(noteId, d, t, srate);
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

Register::Register(std::istream& stream, int srate, MakeFlags& flags) : Generator(stream) {
    if((stream >> skipws).peek() != '{') 
        throw parse_error(stream, "Missing '{'");
    stream.get();
    while((stream >> skipws).peek() != '}') {
        AudioSource* src = AudioSource::Make(stream, srate, flags);
        generators.push_back(src);
    }
    stream.get();
}

AudioSource* Register::copy() {
    std::vector<AudioSource*> gs;
    for(AudioSource* g : generators) {
        gs.emplace_back(g->copy());
    }
    return new Register(gs);
}
