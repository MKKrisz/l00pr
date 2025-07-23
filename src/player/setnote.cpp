#include "setnote.hpp"
#include "../util.hpp"
#include "../tune/tune.hpp"

SetterNote::SetterNote(Source* gen) : Note(), gen(gen){}

SetterNote::SetterNote(std::istream& str, Tune* tune, int srate) : Note() {
    gen = tune->getSource(str);
}
void SetterNote::Write(std::ostream& str) const {
    str << "set(";
    gen->Write(str);
    str << ")";
}
