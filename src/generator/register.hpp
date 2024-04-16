#ifndef L00PR_GENERATORS_REGISTER
#define L00PR_GENERATORS_REGISTER

#include "generator.hpp"

class Register :public Generator {
    std::vector<Generator*> generators;

public:
    Register(std::vector<Generator*> gen);

    double operator()(int noteId, double delta, double t);
    void addPhase();
    void removePhase(int id);
    inline double getSample(double, double) {return 0;}
    Register(std::istream& stream);
    Generator* copy();

    ~Register();
};

#endif
