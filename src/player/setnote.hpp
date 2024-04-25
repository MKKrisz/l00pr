#ifndef L00PR_NOTE_SETTER
#define L00PR_NOTE_SETTER

#include "../generator/generator.hpp"
#include <istream>

/// A special note that when put on a player changes the player's generator
class SetterNote {
    int srcId = -1;
    AudioSource* gen = nullptr;
public:
    SetterNote(int id);
    SetterNote(AudioSource* ptr);
    SetterNote(std::istream& str, int srate = 44100);

    ///getters/setters
    inline int getId() { return srcId; }
    inline AudioSource* getGen() { return gen; }
    inline void setGen(AudioSource* gen) { this->gen = gen; }
};

#endif
