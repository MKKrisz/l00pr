#pragma once

#include "generator.hpp"

//TODO: Consider making a pointer to a copy of this the default argument for most Source* values instead of nullptr
/// <summary> 
/// Generates a constant 0 sample. Serves no purpose, you can't even make one.
/// </summary>
class NoteAmplitude : public Generator {
public: 
    NoteAmplitude();
    NoteAmplitude(const NoteAmplitude& other);

    double getSample(double, double, int) override {return 0;}
    double getSingleSample(double, double, int) override {return 0;}
    void operator()(double phase, double t, int srate, double note_amplitude) override;

    std::unique_ptr<Source> copy() override {return std::make_unique<NoteAmplitude>(*this);}
    std::string ToString() const override { return "NoteAmplitude"; }
    void Write(std::ostream& str) const override {str << "note_amplitude";}

    static std::unique_ptr<NoteAmplitude> Create(std::istream& str, const int, const MakeFlags&);
};
