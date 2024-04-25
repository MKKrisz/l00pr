#ifndef L00PR_NOTEPLAYER
#define L00PR_NOTEPLAYER

#include "../generator/generator.hpp"
#include "note.hpp"
#include "setnote.hpp"

#include <vector>

// Plays notes
class NotePlayer {

    /// The notes the player is currently playing
    std::vector<Note> m_notes;

    /// The current generator
    AudioSource* m_src;

    /// The default generator this player was created with
    AudioSource* def_src;

public:
    /// Adds a note to be played
    void addNote(Note& note);
    void addNote(SetterNote& note);

    NotePlayer(AudioSource* gen);
    NotePlayer(const NotePlayer& player);
    ~NotePlayer();

    /// Returns the current sample value. Called by callback
    float getSample(double srate = 48000);

    NotePlayer& operator=(const NotePlayer& player);
};

#endif
