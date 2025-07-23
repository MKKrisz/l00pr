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

    std::unique_ptr<Source> copy() override { return std::make_unique<SampledGenerator>(*this); }
    std::string ToString() const override { return "Samples from " + filename; }

    static std::unique_ptr<SampledGenerator> Create(std::istream& stream, const int, const MakeFlags&) {
        return std::make_unique<SampledGenerator>(stream);
    }
    
    void operator()(double, double t, int, double note_amplitude) override {
        m_accumulator += (samples[mod(int((t * m_phasemul(t) + m_phaseoffset(t)) * timestep), samples.size())] * m_gain(t) * note_amplitude);
    }

    void Write(std::ostream& str) const override { str << "sampled(" << filename << ")";}
};

#endif
