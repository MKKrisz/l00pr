#ifndef L00PR_GENERATORS_REGISTER
#define L00PR_GENERATORS_REGISTER

#include "generator.hpp"

/// <summary> Funky generator that actually isn't one, it just stores other generators (and possibly their filter chain) </summary>
class Register : public Generator {
    /// <summary> The generators that this Register holds. </summary>
    std::vector<AudioSource*> generators;

    Register(std::vector<AudioSource*> gen);
public:

    void operator()(size_t noteId, double delta, double t, double srate, double extmul);
    double calc();
    void addPhase();
    void removePhase(int id);
    inline double getSample(double, double) {return 0;}
    Register(std::istream& stream, const int, const MakeFlags& = MakeFlags::all);
    Register* copy();
    std::string ToString() { 
        std::string str = "Register: {";
        for(size_t i = 0; i < generators.size(); i++) str += (i > 0 ? " " : "") + generators[i]->ToString();
        return str + "}";
    }

    ~Register();


    static Register* Create(std::istream& str, const int srate, const MakeFlags& flags);
};


#endif
