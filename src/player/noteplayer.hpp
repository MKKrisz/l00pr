#ifndef L00PR_NOTEPLAYER
#define L00PR_NOTEPLAYER

#include "../audiosource.hpp"

#include <vector>

class Note;

// <summary> Structure to hold and play currently playing notes. </summary>
// <remarks> Can't be declared alone, and altough technically doesn't store the entire stream, still requires one to function. (see: lane.hpp, notestream.hpp)</remarks>
class NotePlayer {
    /// <summary> The current source </summary>
    Source* m_src;

    /// <summary> The default generator this player was created with </summary>
    Source* def_src;

public:
    void setSrc(Source* src);
    Source* getSrc() const { return m_src; }

    /// <summary> Adds a note to be played </summary>
    void addNote(std::unique_ptr<Note> note);

    // cctors
    NotePlayer(Source* gen);
    NotePlayer(const NotePlayer& player);

    // dtor
    ~NotePlayer() {}

    /// <summary> Returns the current sample value, and sets itself up for the next sample generation.</summary>
    float getSample(int srate = 48000);

    // copy assgn op.
    NotePlayer& operator=(const NotePlayer& player);
    void operator()(double phase, double t, int srate, double ampl) {
        (*m_src)(phase, t, srate, ampl);
    }


};

#endif
