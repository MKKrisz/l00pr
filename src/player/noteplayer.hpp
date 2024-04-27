#ifndef L00PR_NOTEPLAYER
#define L00PR_NOTEPLAYER

#include "../audiosource.hpp"
#include "note.hpp"
#include "setnote.hpp"

#include <vector>

// <summary> Structure to hold and play currently playing notes. </summary>
// <remarks> Can't be declared alone, and altough technically doesn't store the entire stream, still requires one to function. (see: lane.hpp, notestream.hpp)</remarks>
class NotePlayer {

    /// <summary> The notes the player is currently playing </summary>
    std::vector<Note> m_notes;

    /// <summary> The current source </summary>
    AudioSource* m_src;

    /// <summary> The default generator this player was created with </summary>
    AudioSource* def_src;

public:
    /// <summary> Adds a note to be played </summary>
    void addNote(Note& note);

    /// <summary> Ultimately, sets the current generator to a copy of this note's generator </summary>
    void addNote(SetterNote& note);

    // cctors
    NotePlayer(AudioSource* gen);
    NotePlayer(const NotePlayer& player);

    // dtor
    ~NotePlayer();

    /// <summary> Returns the current sample value, and sets itself up for the next sample generation.</summary>
    float getSample(double srate = 48000);

    // copy assgn op.
    NotePlayer& operator=(const NotePlayer& player);
};

#endif
