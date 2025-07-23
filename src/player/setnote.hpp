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
    /// <summary> The generator to set the player to use. </summary>
    Source* gen = nullptr;
public:
    // cctors
    SetterNote(Source* ptr);
    SetterNote(const SetterNote& s) : gen(s.gen) {}

    /// <summary> Parser for setter notes </summary>
    /// <param name="srate"> Used for accurate filter setup </param>
    SetterNote(std::istream& str, Tune* tune, int srate = 44100);
    ~SetterNote() {}

    //getters/setters

    /// <summary> Returns the note's generator </summary>
    inline Source* getGen() { return gen; }

    /// <summary> Returns the note's generator of nullptr if not set </summary>
    inline void setGen(Source* gen) { this->gen = gen; }

    inline void AddToPlayer(NotePlayer& p) override{
        p.setSrc(getGen());
    }
    
    inline bool IsComplete() const override { return true; }
    inline double GetLen() const override { return 0; }
    inline void AddSample(Source*, int) override { }
    inline std::unique_ptr<Note> copy() const override { return std::make_unique<SetterNote>(*this); }
    std::string ToString() const override { return "\n[" + gen->ToString() + "]"; }

    void Write(std::ostream&) const override;

    static std::unique_ptr<SetterNote> Create(std::istream& str, Tune* tune, double, bool, int srate = 44100) {
        return std::make_unique<SetterNote>(str, tune, srate);
    }
};

#endif
