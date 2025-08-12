#include "loop.hpp"

Loop::Loop(NoteStream& s, double r) : NoteStream(s), repAmount(r) {calculateLen();} 
Loop::Loop(const Loop& l) 
    : NoteStream(l), Note(), repAmount(l.repAmount), t(l.t), reps(l.reps), id(l.id) {}

Loop::Loop(std::istream& str, Tune* tune, double bpm, bool poly, int srate) {
    setBpm(bpm); setPolynote(poly);
    if((str >> skipws).peek() == '(') {
        str.get();
        str >> repAmount;
        str >> expect(')');
    }
    str >> expect('{');
    NoteStream nstr = NoteStream(str, tune, bpm, poly, srate);
    *this = Loop(nstr, repAmount);
    str >> expect('}');
}

void Loop::AddSample(Source* src, int srate) {
    if(IsComplete()) 
        return;

    for( ; id < notes.size(); id++) {
        if(notes[id].first > t) 
            break;

        src->addNote(notes[id].second->copy());
    }
    t += 1.0/srate;
    if(t > len) {
        t = fmod(t, len);
        reps++;
        id = 0;
    }
}

void Loop::Write(std::ostream& str) const {
    str << "loop(" << (repAmount < 0 ? "" : std::to_string(repAmount)) << ") {"; // << std::endl; - Not needed as NoteStream puts one there for free
    NoteStream::Write(str);
    str << std::endl << '}';
}
