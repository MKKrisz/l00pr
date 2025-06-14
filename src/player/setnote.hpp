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
    AudioSource* gen = nullptr;
    bool owner = false;
public:
    // cctors
    SetterNote(AudioSource* ptr);
    SetterNote(const SetterNote& s) : gen(s.gen) {}

    /// <summary> Parser for setter notes </summary>
    /// <param name="srate"> Used for accurate filter setup </param>
    SetterNote(std::istream& str, const std::vector<AudioSource*>& gens, int srate = 44100);
    ~SetterNote() {if(owner) delete gen;}

    //getters/setters

    /// <summary> Returns the note's generator </summary>
    inline AudioSource* getGen() { return gen; }

    /// <summary> Returns the note's generator of nullptr if not set </summary>
    inline void setGen(AudioSource* gen) { this->gen = gen; }

    inline void AddToPlayer(NotePlayer& p) override{
        p.setSrc(getGen());
    }
    
    inline bool IsComplete() const override { return true; }
    inline double GetLen() const override { return 0; }
    inline void AddSample(NotePlayer&, size_t, int) override { }
    inline Note* copy() const override { return new SetterNote(*this); }
    std::string ToString() const override { return "\n[" + gen->ToString() + "]"; }

    void Write(std::ostream&) const override;

    static SetterNote* Create(std::istream& str, const std::vector<AudioSource*>& sources, double, bool, int srate = 44100) {
        return new SetterNote(str, sources, srate);
    }
};

#endif
