#ifndef L00PR_NOTE_SETTER
#define L00PR_NOTE_SETTER

#include "../audiosource.hpp"
#include "note.hpp"
#include <istream>

class SetterNote;

template <typename T>
struct is_setter : std::false_type {};

template <>
struct is_setter<SetterNote> : std::true_type {};

/// <summary> 
/// A special note that when put on a player changes the player's generator 
/// Syntax: set or set(<generator_id>) or set(<generator>)
/// </summary>
/// <remarks> In the first type of declaration it resets the player to its default generator (the one it's been declared with) </remarks>
class SetterNote : public Note{

    /// <summary> If the note was declared using an id, it gets stored here </summary>
    int srcId = -1;

    /// <summary> The generator to set the player to use. </summary>
    AudioSource* gen = nullptr;
public:
    // cctors
    SetterNote(int id);
    SetterNote(AudioSource* ptr);
    SetterNote(const SetterNote& s) : srcId(s.srcId), gen(s.gen) {}

    /// <summary> Parser for setter notes </summary>
    /// <param name="srate"> Used for accurate filter setup </param>
    SetterNote(std::istream& str, int srate = 44100);
    ~SetterNote() {};

    //getters/setters

    /// <summary> Gets the generator id. </summary>
    /// <returns> The id or -1 on failure </returns>
    inline int getId() { return srcId; }

    /// <summary> Returns the note's generator </summary>
    inline AudioSource* getGen() { return gen; }

    /// <summary> Returns the note's generator of nullptr if not set </summary>
    inline void setGen(AudioSource* gen) { this->gen = gen; }

    inline void AddToPlayer(NotePlayer& p) override{
        p.setSrc(getGen()->copy());
    }
    
    inline bool IsComplete() override { return true; }
    inline double GetLen() override { return 0; }
    inline void AddSample(NotePlayer&, size_t, int) override { }
    inline Note* copy() override { return new SetterNote(*this); }
    std::string ToString() override { return "\n[" + gen->ToString() + "]"; }
};

#endif
