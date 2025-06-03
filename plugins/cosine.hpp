#pragma once

#include "../src/generator/generator.hpp"
#include "../src/plugin.hpp"


class CosineGenerator :public Generator {
public: 
    CosineGenerator(Interpolated<double> amplitude = 1.0f,
                  Interpolated<double> phasemul = 1.0f, 
                  Interpolated<double> offset = 0.0f);
    CosineGenerator(const CosineGenerator& g);

    CosineGenerator(std::istream& stream);
    double getSample(double, double) override;
    std::unique_ptr<AudioSource> copy() override;
    std::string ToString() override { return "Cosine"; }


    static std::unique_ptr<CosineGenerator> Create(std::istream& stream, const int, const MakeFlags&);
};

extern "C" { 
void Init(Plugin*); 
}
