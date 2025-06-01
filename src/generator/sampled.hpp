#ifndef L00PR_GENERATORS_SAMPLED
#define L00PR_GENERATORS_SAMPLED

#include "generator.hpp"

class SampledGenerator : public Generator {
    std::vector<double> samples {};
    std::string filename;
    double timestep;

    void parse_file(const std::string& filename);

    size_t mod(int a, int b) {
        return (a % b) + (a < 0 ? b : 0);
    }

public:
    SampledGenerator(std::string sampleFile,
                  Interpolated<double> phase = 1.0f,
                  Interpolated<double> amplitude = 1.0f, 
                  Interpolated<double> offset = 0.0f);
    SampledGenerator(const SampledGenerator&);
    SampledGenerator(std::istream&);

    double getSample(double, double) override;

    std::unique_ptr<AudioSource> copy() override { return std::make_unique<SampledGenerator>(*this); }
    std::string ToString() override { return "Samples from " + filename; }

    static std::unique_ptr<SampledGenerator> Create(std::istream& stream, const int, const MakeFlags&) {
        return std::make_unique<SampledGenerator>(stream);
    }
    
    void operator()(size_t noteId, double delta, double t, double srate, double extmul) override {
#ifdef DEBUG
        if(noteId >= phases.size)
            throw std::out_of_range("SampledGenerator phases");
#endif
        if (getLengthBounds().has_value() && t > getLengthBounds().value().second) return;
        double& phase = phases[noteId];
        accumulator += (samples[mod(int((phase + m_phaseoffset(t))/timestep), samples.size())] * m_gain(t) * extmul);
        phase += delta * m_phasemul(t);
    }
};

#endif
