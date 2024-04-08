#ifndef L00PR_LOOP
#define L00PR_LOOP

#include "notestream.hpp"

class Loop : public NoteStream {
    float repAmount = -1;
public:
    float getRepAmount() { return repAmount; }

    Loop(NoteStream&, float);
    Loop(std::istream& str, float bpm, bool poly);

    std::vector<Note> getStartingPlayableNotes(float t);

    std::vector<SetterNote> getStartingSetterNotes(float t);
};

#endif
