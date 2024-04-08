#include "loop.hpp"

Loop::Loop(NoteStream& s, float r) : NoteStream(s), repAmount(r) {} 

Loop::Loop(std::istream& str) {
    if((str >> std::ws).peek() == '(') {
        str.get();
        str >> repAmount;
        if((str >> std::ws).peek() != ')') {
            throw parse_error(str, "Expected ')'");
        }
        str.get();
    }
    if((str >> std::ws).peek() != '{') 
        throw parse_error(str, "Expected '{'");
    str.get();
    str >> *this;
    if((str >> std::ws).peek() != '}') 
        throw parse_error(str, "Expected '}'");
    str.get();
}

std::vector<Note> Loop::getStartingPlayableNotes(float t) {
    if(playable.empty()) return {};
    if(repAmount != -1 && t > getLen()*repAmount) return {};
    t = fmod(t, getLen());
    std::vector<Note> ret;
    auto i = playable.begin();
    for( ; i != playable.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    playable.erase(playable.begin(), i);
    return ret;
}

std::vector<SetterNote> Loop::getStartingSetterNotes(float t) {
    if(setter.empty()) return {};
    if(repAmount != -1 && t > getLen()*repAmount) return {};
    t = fmod(t, getLen());
    std::vector<SetterNote> ret;
    auto i = setter.begin();
    for( ; i != setter.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    setter.erase(setter.begin(), i);
    return ret;

}
