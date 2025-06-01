#include "setnote.hpp"
#include "../util.hpp"

SetterNote::SetterNote(AudioSource* gen) : gen(gen){}

SetterNote::SetterNote(std::istream& str, const std::vector<AudioSource*>& gens, int srate){
    str >> expect('(');
    str >>skipws;
    if(isdigit(str.peek())) {
        int id;
        str >> id;
        gen = gens[id];
    }
    else {
        std::string name = "";
        std::getline(str, name, ')');
        gen = AudioSource::getByName(gens, name);
    }
    str >> expect(')');
}
