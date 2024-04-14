#include "util.hpp"
#include "exceptions/parse_error.hpp"

#include <cmath>
#include <string>
#include <iostream>

float getFreq(const char* s, int* n) {
    int note = -1; //from 0 to 12 the offset of the note
    int octave = -1;
    bool sharp = false; //wether 

    // initialize n
    if(n != nullptr) *n = 0;

    if(isdigit(s[0])){ 

        // skip numeric characters and decimal points
        int i = 0;
        while(isdigit(s[i]) || s[i] == '.') i++;

        if(n != nullptr) *n = i;
        return atof(s);
    }

    // early return
    if(s[0] < 'A' || s[0] > 'G') return 0;
    note = (s[0] -'A')*2;       //

    // remove inaccuracy caused by the missing black keys between B-C and E-F
    if(s[0] > 'B') note--;
    if(s[0] > 'E') note--;

    // handling modifiers
    if(s[1] == '#')      {note++;    sharp = true;}
    else if(s[1] == 'x') {note += 2; sharp = true;}
    else if(s[1] == 'b') {note--;    sharp = true;}

    // return if next char is not a digit
    else if(!isdigit(s[sharp?2:1])) return 0;

    octave = s[sharp?2:1] - '0';

    // since we caluclate every note upwards from 'A', notes would get one 
    // octave higher if not for this
    if(s[0] > 'B') octave--;
    if(n != nullptr) { *n = sharp?3:2; } //setting n
    
    // crazy calculation explained one function below
    return 440*pow(2.0f, (octave-4) + note/12.0f);
}

double getFreq(std::istream& stream) {
    //skips whitespace then checks if next char is a number
    if(isdigit((stream >> skipws).peek())) {
        double ret;
        stream >> ret;
        return ret;
    }
    char buf;
    int octave;
    int note;
    stream >> buf;
    
    if(buf < 'A' || buf > 'G')
        throw parse_error(stream, "Couldn't interpret stream as a note or float");

    //same thing as above except in a tightly packed ternary-chain
    note = buf > 'E' ? (buf-'A')*2 - 2 : 
           buf > 'B' ? (buf-'A')*2 - 1 : (buf-'A')*2;

    //handling the modifier
    buf = stream.peek();
    if(buf == '#')      { note++;  stream.get(); }
    else if(buf == 'b') { note--;  stream.get(); }
    else if(buf == 'x') { note+=2; stream.get(); }

    if(isdigit(stream.peek())){
        stream >> octave;
        if(note > 2) octave--;
    }
    else throw parse_error(stream, "Couldn't interpret stream as a note or float");
    
    // A4 is 440Hz (pls don't argue, accept that it is)
    // In equal temperament the quotient of adjacent notes is 2^(1/12)
    // from that it is easy to conclude that the formula for caluclating the 
    // frequency of the given note is (considering our reference point of A4):
    //
    // 440 * 2^(octave - 4 + (note/12))
    //
    return 440 * pow(2.0f, (octave-4) + note/12.0f);
}


std::pair<int, int> getLineColNo(std::istream& stream) {
    // clear the bad bit
    stream.clear();
    int pos = stream.tellg();
    stream.seekg(0);
    stream >> std::noskipws;

    int lineno = 1;
    int colno = 1;
    char buf;
    while(stream.tellg() < pos && stream>>buf) {
        if(buf == '\n') {
            lineno++; 
            colno = 1;
            continue;
        } 
        colno++;
    }
    return std::make_pair(lineno, colno);
}

std::istream& skipws(std::istream& str) {
    while((str >> std::ws).peek() == '#') {
        char c;
        while(str.get(c) && c != '\n') {}
    }
    return str;
}
