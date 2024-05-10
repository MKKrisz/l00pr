#ifndef L00PR_NOTEPLAYER
#define L00PR_NOTEPLAYER

#include "../audiosource.hpp"

#include <vector>

class Note;

// <summary> Structure to hold and play currently playing notes. </summary>
// <remarks> Can't be declared alone, and altough technically doesn't store the entire stream, still requires one to function. (see: lane.hpp, notestream.hpp)</remarks>
class NotePlayer {

    /// <summary> The notes the player is currently playing </summary>
    std::vector<Note*> m_notes;

    /// <summary> The current source </summary>
    AudioSource* m_src;

    /// <summary> The default generator this player was created with </summary>
    AudioSource* def_src;

public:
    void setSrc(AudioSource* src);
    inline bool hasBounds() { return m_src->getLengthBounds().has_value(); }
    inline std::pair<double, double> getBounds() { return m_src->getLengthBounds().value(); }
    /// <summary> Adds a note to be played </summary>
    void addNote(Note* note);

    // cctors
    NotePlayer(AudioSource* gen);
    NotePlayer(const NotePlayer& player);

    // dtor
    ~NotePlayer();

    /// <summary> Returns the current sample value, and sets itself up for the next sample generation.</summary>
    float getSample(double srate = 48000);

    // copy assgn op.
    NotePlayer& operator=(const NotePlayer& player);
    void operator()(size_t id, double delta, double t, int srate, double ampl) {
        (*m_src)(id, delta, t, srate, ampl);
    }


};

#endif
