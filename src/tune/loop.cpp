#include "loop.hpp"

Loop::Loop(NoteStream& s, double r) : NoteStream(s), repAmount(r) {} 
Loop::Loop(const Loop& l) 
    : NoteStream(l), repAmount(l.repAmount), len(l.len), t(l.t), reps(l.reps), id(l.id) {}

Loop::Loop(std::istream& str, const std::vector<AudioSource*>& sources, double bpm, bool poly, int srate) {
    setBpm(bpm); setPolynote(poly);
    if((str >> skipws).peek() == '(') {
        str.get();
        str >> repAmount;
        str >> expect(')');
    }
    str >> expect('{');
    NoteStream nstr = NoteStream(str, sources, bpm, poly, srate);
    *this = Loop(nstr, repAmount);
    str >> expect('}');
}

void Loop::AddSample(NotePlayer& p, size_t, int) {
    if(repAmount >= 0 && IsComplete()) 
        return;

    for( ; id < notes.size(); id++) {
        if(notes[id].first > t) 
            break;

        p.addNote(notes[id].second->copy());
    }
    t += 1.0/srate;
    if(t > len) {
        t = fmod(t, len);
        reps++;
        id = 0;
    }
}

