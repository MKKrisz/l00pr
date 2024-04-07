#ifndef L00PR_GENERATORS_REGISTER
#define L00PR_GENERATORS_REGISTER

#include "../generator.hpp"

class Register :public Generator {
    std::vector<Generator*> generators;

public:
    Register(std::vector<Generator*> gen);

    float operator()(int noteId, float delta, float t);
    void addPhase();
    void removePhase(int id);
    inline float getSample(float phase, float t) {return 0;}
    Register(std::istream& stream);
    Generator* copy();

    ~Register();
};

#endif
