#pragma once

#include "generator.hpp"

class ValueGenerator : public Generator {
public:
    ValueGenerator(Interpolated<double> amplitude = 0.0);
    ValueGenerator(const ValueGenerator& g);

    ValueGenerator(std::istream& stream);
    double getSample(double, double) override;
    std::unique_ptr<Source> copy() override;
    std::string ToString() const override { return "Value"; }
    void Write(std::ostream& str) const override;


    static std::unique_ptr<ValueGenerator> Create(std::istream& stream, const int, const MakeFlags&);
    static std::unique_ptr<ValueGenerator> CreateAsDefault(std::istream& stream, const int, const MakeFlags&);
};
