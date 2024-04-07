#include "register.hpp"

Register::Register(std::vector<Generator*> gen) : generators(gen) {}

float Register::operator()(int noteId, float d, float t) {
    float sum = 0;
    for(Generator* g : generators) {
        sum += (*g)(noteId, d, t);
    }
    return sum;
}

void Register::addPhase() {
    for(Generator* g : generators) {
        g->addPhase();
    }
}

void Register::removePhase(int id) {
    for(Generator* g : generators) {
        g->removePhase(id);
    }
}

Register::~Register() {
    for(Generator* g : generators) {
        delete g;
    }
}

Register::Register(std::istream& stream) {
    if((stream >> std::ws).peek() != '{') 
        throw parse_error(stream, "Missing '{'");
    stream.get();
    while((stream >> std::ws).peek() != '}') {
        Generator* gen;
        stream >> &gen;
        generators.push_back(gen);
    }
    stream.get();
}

Generator* Register::copy() {
    std::vector<Generator*> gs;
    for(Generator* g : generators) {
        gs.emplace_back(g->copy());
    }
    return new Register(gs);
}
