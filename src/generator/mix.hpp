#pragma once
#include "generator.hpp"

/// <summary> Funky generator that actually isn't one, it just stores other generators (and possibly their filter chain) </summary>
class Mixer : public Generator {
    /// <summary> The generators that this Mixer holds. </summary>
    std::vector<std::unique_ptr<Source>> generators {};
    double actual_amplitude = 0;

public:
    void operator()(double phase, double t, int srate, double note_amplitude);
    double getSample(int srate);
    void addNote(std::unique_ptr<Note>);
    std::vector<SourceRef*> getSourceRefs();
    std::vector<Source*> getLabeled();

    double getSample(double, double, int) {return 0;}
    double getSingleSample(double, double, int);
    Mixer(std::vector<Source*> gen);
    Mixer(std::istream& stream, const int, const MakeFlags& = MakeFlags::all);
    Mixer(const Mixer& r);

    std::unique_ptr<Source> copy();
    std::string ToString() const { 
        std::string str = "Mixer: {";
        for(size_t i = 0; i < generators.size(); i++) str += (i > 0 ? " " : "") + generators[i]->ToString();
        return str + "}";
    }
    void Write(std::ostream& str) const;

    static std::unique_ptr<Mixer> Create(std::istream& str, const int srate, const MakeFlags& flags);
};
