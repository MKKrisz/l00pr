#ifndef L00PR_LOOP
#define L00PR_LOOP

#include "notestream.hpp"
#include "../player/note.hpp"

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
class Loop : public NoteStream, Note {
    /// <summary> The amount the loop needs to repeat </summary>
    double repAmount = -1;
    double len = 0;

    double t = 0;
    int reps = 0;
    size_t id = 0;
public:
    /// <summary> Gets how many times the loop needs to repeat </summary>
    double getRepAmount() { return repAmount; }

    // cctors
    Loop(NoteStream&, double);
    Loop(const Loop& l);

    /// <summary> Parser for loops </summary>
    Loop(std::istream& str, const std::vector<AudioSource*>& sources, double bpm, bool poly, int srate);

    NoteStream& getBaseStream() {return *this; }

    void AddToPlayer(NotePlayer&) override {
        len = NoteStream::getLen();
    }
    void AddSample(NotePlayer& p, size_t, int) override;
    double GetLen() override {
        if(repAmount < 0) 
            return std::numeric_limits<double>::infinity();
        return NoteStream::getLen() * repAmount;
    }
    double getLen() { return GetLen(); }
    bool IsComplete() override {
        if(repAmount < 0) return false;
        return (t + reps*len) > repAmount * len;
    }
    Note* copy() override {return new Loop(*this);}
    Loop& operator=(const Loop& l) {
        if(this == &l) return *this;
        NoteStream::operator=(l);
        repAmount = l.repAmount;
        len = l.len;
        t = l.t;
        reps = l.reps;
        id = l.id;
        return *this;
    };

    std::string ToString() override {
        std::string str = "\n" + (repAmount < 0? "inf" : std::to_string(repAmount)) +"*[\n";
        for(size_t i = 0; i < notes.size(); i++) {
            str += "\t" + std::to_string(notes[i].first) + ": " + notes[i].second->ToString() + "\n";
        }
        str += "]\n";
        return str;
    }
};

#endif
