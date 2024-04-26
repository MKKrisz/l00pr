#ifndef L00PR_GENERATORS_REGISTER
#define L00PR_GENERATORS_REGISTER

#include "generator.hpp"

class Register : public Generator {
    std::vector<AudioSource*> generators;

    Register(std::vector<AudioSource*> gen);
public:

    void operator()(int noteId, double delta, double t, double srate, double extmul);
    double calc();
    void addPhase();
    void removePhase(int id);
    inline double getSample(double, double) {return 0;}
    Register(std::istream& stream, const int, const MakeFlags& = MakeFlags::all);
    AudioSource* copy();

    ~Register();
};

#endif
