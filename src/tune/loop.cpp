#include "loop.hpp"

Loop::Loop(NoteStream& s, double r) : NoteStream(s), repAmount(r) {} 

Loop::Loop(std::istream& str, double bpm, bool poly) {
    setBpm(bpm); setPolynote(poly);
    if((str >> skipws).peek() == '(') {
        str.get();
        str >> repAmount;
        if((str >> skipws).peek() != ')') {
            throw parse_error(str, "Expected ')'");
        }
        str.get();
    }
    if((str >> skipws).peek() != '{') 
        throw parse_error(str, "Expected '{'");
    str.get();
    str >> *this;
    if((str >> skipws).peek() != '}') 
        throw parse_error(str, "Expected '}'");
    str.get();
}

std::vector<Note> Loop::getStartingPlayableNotes(double t) {
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

std::vector<SetterNote> Loop::getStartingSetterNotes(double t) {
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
