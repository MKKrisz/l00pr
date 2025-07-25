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
    SampledGenerator(std::istream&, int srate);

    double getSample(double, double, int) override;

    std::unique_ptr<Source> copy() override { return std::make_unique<SampledGenerator>(*this); }
    std::string ToString() const override { return "Samples from " + filename; }

    static std::unique_ptr<SampledGenerator> Create(std::istream& stream, const int srate, const MakeFlags&) {
        return std::make_unique<SampledGenerator>(stream, srate);
    }
    
    void operator()(double, double t, int srate, double note_amplitude) override {
        (*m_phasemul)(t, t, srate, 1);
        (*m_phaseoffset)(t, t, srate, 1);
        (*m_gain)(t, t, srate, 1);

        m_accumulator += (samples[mod(int((t * m_phasemul->getSample(srate) + m_phaseoffset->getSample(srate)) * timestep), samples.size())] * m_gain->getSample(srate) * note_amplitude);
    }

    void Write(std::ostream& str) const override { str << "sampled(" << filename << ")";}
};

#endif
