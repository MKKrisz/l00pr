#pragma once

#include "generator.hpp"

//TODO: Consider making a pointer to a copy of this the default argument for most Source* values instead of nullptr
/// <summary> 
/// Generates a constant 0 sample. Serves no purpose, you can't even make one.
/// </summary>
class NoteCount : public Generator {
public: 
    NoteCount();
    NoteCount(const NoteCount& other);

    double getSample(double, double, int) override {return 0;}
    double getSingleSample(double, double, int) override {return 0;}
    double getSample(int samplerate) override;

    std::unique_ptr<Source> copy() override {return std::make_unique<NoteCount>(*this);}
    std::string ToString() const override { return "NoteCount"; }
    void Write(std::ostream& str) const override {str << "note_amplitude";}

    static std::unique_ptr<NoteCount> Create(std::istream& str, const int, const MakeFlags&);
};
