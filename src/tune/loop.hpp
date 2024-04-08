#ifndef L00PR_LOOP
#define L00PR_LOOP

#include "notestream.hpp"

class Loop : public NoteStream {
    double repAmount = -1;
public:
    double getRepAmount() { return repAmount; }

    Loop(NoteStream&, double);
    Loop(std::istream& str, double bpm, bool poly);

    std::vector<Note> getStartingPlayableNotes(double t);

    std::vector<SetterNote> getStartingSetterNotes(double t);
};

#endif
