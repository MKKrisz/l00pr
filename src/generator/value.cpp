#include "value.hpp"
#include "../util.hpp"

#include <cmath>

ValueGenerator::ValueGenerator(Interpolated<double> amplitude) : Generator(1.0, amplitude, 0.0){}


ValueGenerator::ValueGenerator(const ValueGenerator& g) : Generator(g) {}

double ValueGenerator::getSample(double, double t) {
    return m_gain(t);
}

ValueGenerator::ValueGenerator(std::istream& stream) : Generator() {
    if((stream >> skipws).peek() != '('){ return; }
    stream.get();
    m_gain.Clear();
    stream >> skipws >> m_gain >> expect(')');
}

std::unique_ptr<Source> ValueGenerator::copy() { 
    return std::make_unique<ValueGenerator>(*this);
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
    m_gain.Write(str);
    str << ") ";
}
