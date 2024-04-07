#ifndef L00PR_NOTE_SETTER
#define L00PR_NOTE_SETTER

#include "../generator/generator.hpp"
#include <istream>

/// A special note that when put on a player changes the player's generator
class SetterNote {
    int generatorId = -1;
    Generator* gen = nullptr;
public:
    SetterNote(int id);
    SetterNote(Generator* ptr);
    SetterNote(std::istream& str);

    ///getters/setters
    inline int getId() { return generatorId; }
    inline Generator* getGen() { return gen; }
    inline void setGen(Generator* gen) { this->gen = gen; }
};

#endif
