#ifndef L00PR_NOTES_RANDOM
#define L00PR_NOTES_RANDOM

#include <vector>
#include <istream>
#include "../interpolated.hpp"
#include "../util.hpp"
#include "../range.hpp"
#include "note.hpp"
#include "../tune/notestream.hpp"

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

class RandomNote {
    Range<Interpolated<Frequency>> frequencies;
    bool useContinuousFreq = false;
    
    Range<double> lengths;

    Range<Interpolated<double>> amplitudes = {1};

    NoteStream notes;
    bool noteBased = false;

    double len = 0;
    double bpm;
public:
    double getLen() {return len;}
    RandomNote(std::istream&, double);
    std::vector<std::pair<double, Note>> Serialize(double start);
};

#endif
