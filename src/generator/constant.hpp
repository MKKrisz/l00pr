#pragma once

#include "generator.hpp"

class ConstantGenerator : public Generator {
public:
    ConstantGenerator(Interpolated<double> amplitude = 1.0f,
                  Interpolated<double> phasemul = 1.0f, 
                  Interpolated<double> offset = 0.0f);
    ConstantGenerator(const ConstantGenerator& g);

    ConstantGenerator(std::istream& stream);
    double getSample(double, double) override;
    std::unique_ptr<AudioSource> copy() override;
    std::string ToString() override { return "Sine"; }


    static std::unique_ptr<ConstantGenerator> Create(std::istream& stream, const int, const MakeFlags&);
private:
    float m_constant = 0;
};
