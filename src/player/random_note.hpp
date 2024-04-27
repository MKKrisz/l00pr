#ifndef L00PR_NOTES_RANDOM
#define L00PR_NOTES_RANDOM

#include <vector>
#include <istream>
#include "../interpolated.hpp"
#include "../util.hpp"
#include "../range.hpp"
#include "note.hpp"
#include "../tune/notestream.hpp"

//Note to my favourite desser: I'm going to leave these design notes in, just for fun...

//How to characterise random note sequences?
//1, We need to know the sequence's length
//2, We need to be able to define the maximum and minimum lengths for a note in the sequence
//3, We need to be able to provide either the fix frequencies it can produce or the bounds.
//4, We need to be able to define maximum and minimum amplitudes
//
//Ranges: (v1, v2, v3, ..., vk)     -- this can have interpolated values
//              OR
//        (vmin -- vmax)         -- this cannot have interpolated values
//              OR just v
//
//Overall syntax:
//      <timestamp random(freqs len lens ampls)>
//              OR
//      <timestmp random(len){ notes }>

/// <summary> 
/// Generates a random note sequence. 
/// Syntax: random(<frequencies> <full_length> <lengths> <amplitudes>) 
/// or random(<length>){<note1> <note2> ... <note_n>}
/// </summary>
class RandomNote {
    /// <summary> The frequencies the object can choose from </summary>
    Range<Interpolated<Frequency>> frequencies;
    //TODO: implement note-based frequency interpolation
    bool useContinuousFreq = false;
    
    /// <summary> The lengths the object can choose from when generating </summary>
    Range<double> lengths;

    /// <summary> The amplitude values the object can choose from </summary>
    Range<Interpolated<double>> amplitudes = {1};

    /// <summary> The notes the object can choose from (if declared using premade notes) </summary>
    NoteStream notes;

    /// <summary> Flag set when declared using notes </summary>
    bool noteBased = false;

    //TODO: calculate this to be the ending point, not the start
    /// <summary> Latest point in time when a note of arbitrary length can start </summary>
    double len = 0;

    /// <summary> The bpm value to interpret note lengths with. </summary> 
    double bpm;

public:

    /// <summary> Returns the "length" of this sequence </summary>
    double getLen() {return len;}

    // cctor
    RandomNote(std::istream&, double);

    /// <summary> Generates the actual note sequence that gets played</summary>
    std::vector<std::pair<double, Note>> Serialize(double start);
};

#endif
