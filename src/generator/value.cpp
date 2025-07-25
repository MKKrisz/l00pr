#include "value.hpp"
#include "../util.hpp"
#include "../player/note.hpp"

ValueGenerator::ValueGenerator(Interpolated<double> amplitude) : Generator(true), m_value(amplitude){}


ValueGenerator::ValueGenerator(const ValueGenerator& g) : m_value(g.m_value) {
    playing_notes.reserve(g.playing_notes.size());
    for(const auto& note : g.playing_notes) { playing_notes.emplace_back(note->copy()); }
}

double ValueGenerator::getSample(double, double t, int) {
    return m_value(t);
}

ValueGenerator::ValueGenerator(std::istream& stream) : Generator(), m_value() {
    if((stream >> skipws).peek() != '('){ return; }
    stream.get();
    m_value.Clear();
    stream >> skipws >> m_value >> expect(')');
}

std::unique_ptr<Source> ValueGenerator::copy() { 
    return std::make_unique<ValueGenerator>(*this);
}
void ValueGenerator::operator()(double phase, double t, int srate, double note_amplitude) {
    m_accumulator += getSample(phase, t, srate) * note_amplitude;
}

std::unique_ptr<ValueGenerator> ValueGenerator::Create(std::istream& stream, const int, const MakeFlags&) {
    return std::make_unique<ValueGenerator>(stream);
}

std::unique_ptr<ValueGenerator> ValueGenerator::CreateAsDefault(std::istream& stream, const int, const MakeFlags&) {
    Interpolated<double> value;
    if(!isdigit((stream >> skipws).peek())) { throw no_such_keyword(); }
    stream >> value;
    return std::make_unique<ValueGenerator>(value);
}

void ValueGenerator::Write(std::ostream& str) const {
    str << "constant("; 
    m_value.Write(str);
    str << ") ";
}
