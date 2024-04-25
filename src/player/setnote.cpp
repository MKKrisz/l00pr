#include "setnote.hpp"

SetterNote::SetterNote(int id) : srcId(id) {}
SetterNote::SetterNote(AudioSource* gen) : gen(gen){}

SetterNote::SetterNote(std::istream& str, int srate){
    str >> expect('(');
    str >>skipws;
    if(isdigit(str.peek()))
        str >> srcId;
    else
        gen = AudioSource::Make(str, srate);
    str >> expect(')');
}
