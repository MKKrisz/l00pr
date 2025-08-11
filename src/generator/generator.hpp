#ifndef L00PR_GENERATOR_H
#define L00PR_GENERATOR_H

#include "../audiosource.hpp"
#include "../interpolated.hpp"

struct Gen_Metadata : public Metadata<std::unique_ptr<Source>, const int, const MakeFlags&>{
public:
    std::string syntax;
    std::string desc;
    Gen_Metadata(const char* kw, std::function<std::unique_ptr<Source>(std::istream&, const int, const MakeFlags&)> func, const char* syn, const char* desc) 
        : Metadata(kw, func), syntax(syn), desc(desc) {};
    Gen_Metadata(const Gen_Metadata& meta) = default;
    std::string ToString() const override;

    Gen_Metadata& operator=(const Gen_Metadata& m) = default;
};

/// <summary> Source that actually generates samples </summary>
class Generator : public Source, public Parseable<std::unique_ptr<Source>, Gen_Metadata, const int, const MakeFlags&> {
protected:

    std::vector<std::unique_ptr<Note>> playing_notes;

    /// <summary> The frequency multiplier that should be applied </summary>
    std::unique_ptr<Source> m_phasemul = nullptr;

    /// <summary> The gain multiplier that should be applied </summary>
    std::unique_ptr<Source> m_gain = nullptr;

    /// <summary> The phase offset that should be applied </summary>
    std::unique_ptr<Source> m_phaseoffset = nullptr;

    Generator(bool will_self_initialize);

    // Base constructors for subclasses
    Generator(Interpolated<double> mul = 1.0f, Interpolated<double> gain = 1.0f, Interpolated<double> offs = 0.0f);

    Generator(const Generator& g);
    
    Generator(std::istream&, int);

    /// <summary> Value that indicates that there will be no new arguments to this generator </summary>
    bool shouldBeDefault;
public: 
    static void Init();

    void addNote(std::unique_ptr<Note>) override;

    double getSample(int samplerate) override;

    double getFrequencyMultiplier(double t, int srate) override;
    
    /// <summary> Gets a sample of this generator </summary>
    /// <param name="phase"> A value going from 0 to 1 </param>
    /// <param name="t"> A timestamp so that the member values of type Interpolated<double> can be applied properly </param>
    virtual double getSample(double phase, double t, int srate) = 0;
    double getSingleSample(double phase, double t, int srate) override;

    /// <summary> Generates the sample from this generator with all the modifiers applied, then adds that to the accumulator </summary>
    virtual void operator()(double phase, double t, int srate, double note_amplitude) override;

    virtual ~Generator();

    virtual std::string ToString() const override { return "Generator"; }

    static std::string getFormattedMetadata();

    void WriteBaseParams(std::ostream& str) const;
    void Write(std::ostream& str) const override;
};

#endif
