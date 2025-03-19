#include "util.hpp"
#include "exceptions/parse_error.hpp"

#include <cmath>
#include <string>
#include <iostream>

double getFreq(std::istream& stream) {
    //skips whitespace then checks if next char is a number
    if(isdigit((stream >> skipws).peek()) || stream.peek() == '-') {
        double ret;
        stream >> ret;
        return ret;
    }
    char buf;
    int octave;
    int note;
    stream >> buf;
    
    if(buf == 'H') 
        throw parse_error(stream, "Try using B in place of H. (H becomes B; B becomes Bb)");
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

exp_p expect(char c) {return exp_p{c};}
std::istream& operator>>(std::istream& str, const exp_p& c) {
    str >> skipws;
    if(str.peek() != c.val)
        throw parse_error(str, std::string("Expected '") + c.val + "'");
    str.get();
    return str;
}
