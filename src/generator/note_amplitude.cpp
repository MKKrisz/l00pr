#include "note_amplitude.hpp"

NoteAmplitude::NoteAmplitude() : Generator() {/*nothing*/}

NoteAmplitude::NoteAmplitude(const NoteAmplitude& other) : Generator(other) {/*nothing*/}

void NoteAmplitude::operator()(double, double, int, double note_ampl) {
    m_accumulator += note_ampl;
}


std::unique_ptr<NoteAmplitude> NoteAmplitude::Create(std::istream&, const int, const MakeFlags&) {
    return std::make_unique<NoteAmplitude>(); // no arguments means peace for the mind....
}
