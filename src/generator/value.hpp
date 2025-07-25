#pragma once

#include "generator.hpp"

class ValueGenerator : public Generator {
    Interpolated<double> m_value;
public:
    ValueGenerator(Interpolated<double> amplitude = 0.0);
    ValueGenerator(const ValueGenerator& g);

    ValueGenerator(std::istream& stream);
    double getSample(double, double, int) override;
    double getSingleSample(double p, double t, int s) override {return getSample(p, t, s);}
    double getFrequencyMultiplier(double, int) override { return 1; }
    std::unique_ptr<Source> copy() override;
    std::string ToString() const override { return "Value"; }
    void Write(std::ostream& str) const override;

    void operator()(double phase, double t, int srate, double note_amplitude) override;


    static std::unique_ptr<ValueGenerator> Create(std::istream& stream, const int, const MakeFlags&);
    static std::unique_ptr<ValueGenerator> CreateAsDefault(std::istream& stream, const int, const MakeFlags&);
};
