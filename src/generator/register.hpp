#ifndef L00PR_GENERATORS_REGISTER
#define L00PR_GENERATORS_REGISTER

#include "generator.hpp"

/// <summary> Funky generator that actually isn't one, it just stores other generators (and possibly their filter chain) </summary>
class Register : public Generator {
    /// <summary> The generators that this Register holds. </summary>
    std::vector<std::unique_ptr<Source>> generators {};

public:
    void operator()(double phase, double t, int srate, double note_amplitude);
    double getSample(int srate);
    void addNote(std::unique_ptr<Note>);
    std::vector<SourceRef*> getSourceRefs();
    std::vector<Source*> getLabeled();

    double getSample(double, double) {return 0;}
    Register(std::vector<Source*> gen);
    Register(std::istream& stream, const int, const MakeFlags& = MakeFlags::all);
    Register(const Register& r);

    std::unique_ptr<Source> copy();
    std::string ToString() const { 
        std::string str = "Register: {";
        for(size_t i = 0; i < generators.size(); i++) str += (i > 0 ? " " : "") + generators[i]->ToString();
        return str + "}";
    }
    void Write(std::ostream& str) const;

    static std::unique_ptr<Register> Create(std::istream& str, const int srate, const MakeFlags& flags);
};


#endif
