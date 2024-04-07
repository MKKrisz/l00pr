#ifndef L00PR_NOTEPLAYER
#define L00PR_NOTEPLAYER

#include "../generator/generator.hpp"
#include "note.hpp"
#include "setnote.hpp"

#include <vector>

// Plays notes
class NotePlayer : AudioSource {

    /// The notes the player is currently playing
    std::vector<Note> m_notes;

    /// The current generator
    Generator* m_gen;

    /// The default generator this player was created with
    Generator* def_gen;

public:
    /// Adds a note to be played
    void addNote(Note& note);
    void addNote(SetterNote& note);

    NotePlayer(Generator* gen);
    NotePlayer(const NotePlayer& player);
    ~NotePlayer();

    /// Returns the current sample value. Called by callback
    float getSample(float srate = 48000);

    NotePlayer& operator=(const NotePlayer& player);
};

#endif
