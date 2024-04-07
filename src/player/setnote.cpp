#include "setnote.hpp"
#include "../exceptions/parse_error.hpp"
#include <string>

SetterNote::SetterNote(int id) : generatorId(id) {}
SetterNote::SetterNote(Generator* gen) : gen(gen){}

SetterNote::SetterNote(std::istream& str){
    str >> std::ws;
    if((str >> std::ws).peek() != '(')
        throw parse_error(str, "Missing '('");

    str.get();
    str >> generatorId;
    if((str >> std::ws).peek() != ')')
        throw parse_error(str, "Missing ')'");
    str.get();
}
