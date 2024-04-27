#ifndef L00PR_LOOP
#define L00PR_LOOP

#include "notestream.hpp"

/// <summary> 
/// A loop. What is there to explain ??
/// Syntax: loop{<note1> [note2] ... [note_n]} 
/// or loop(<repeat_count>) {<note1> [note2] ... [note_n]}
/// </summary>
/// <remarks>
/// The first type of declaration creates an infinite loop. (well technically not, but it is still 130 or so years.)
/// DO NOT and I repeat DO NOT TRY TO EXPORT A TUNE WITH AN INFINITE LOOP!!!
/// (in the future, there will be a failsafe for that, currently there is none...)
/// </remarks>
class Loop : public NoteStream {
    /// <summary> The amount the loop needs to repeat </summary>
    double repAmount = -1;
public:
    /// <summary> Gets how many times the loop needs to repeat </summary>
    double getRepAmount() { return repAmount; }

    // cctors
    Loop(NoteStream&, double);

    /// <summary> Parser for loops </summary>
    Loop(std::istream& str, double bpm, bool poly);

    NoteStream& getBaseStream() {return *this; }

    std::vector<Note> getStartingPlayableNotes(double t);

    std::vector<SetterNote> getStartingSetterNotes(double t);
};

#endif
