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
    Gen_Metadata(const Gen_Metadata& meta) : Metadata(meta), syntax(meta.syntax), desc(meta.desc) {}
    std::string ToString() const override;

    Gen_Metadata& operator=(const Gen_Metadata& m) = default;
};

/// <summary> Source that actually generates samples </summary>
class Generator : public Source, public Parseable<std::unique_ptr<Source>, Gen_Metadata, const int, const MakeFlags&> {
protected:

    std::vector<std::unique_ptr<Note>> playing_notes;

    /// <summary> The frequency multiplier that should be applied </summary>
    Interpolated<double> m_phasemul;

    /// <summary> The gain multiplier that should be applied </summary>
    Interpolated<double> m_gain;

    /// <summary> The phase offset that should be applied </summary>
    Interpolated<double> m_phaseoffset;

    // Base constructors for subclasses
    Generator(Interpolated<double> mul = 1.0f, Interpolated<double> gain = 1.0f, Interpolated<double> offs = 0.0f);

    Generator(const Generator& g);
    
    Generator(std::istream&);

    /// <summary> Value that indicates that there will be no new arguments to this generator </summary>
    bool shouldBeDefault;
public: 
    static void Init();

    void addNote(std::unique_ptr<Note>) override;

    double getSample(int samplerate) override;

    double getFrequencyMultiplier(double t) override;
    
    /// <summary> Gets a sample of this generator </summary>
    /// <param name="phase"> A value going from 0 to 1 </param>
    /// <param name="t"> A timestamp so that the member values of type Interpolated<double> can be applied properly </param>
    virtual double getSample(double phase, double t) = 0;

    /// <summary> Generates the sample from this generator with all the modifiers applied, then adds that to the accumulator </summary>
    virtual void operator()(double phase, double t, int srate, double note_amplitude) override;

    virtual ~Generator();

    virtual std::string ToString() const override { return "Generator"; }

    static std::string getFormattedMetadata();

    void WriteBaseParams(std::ostream& str) const;
    void Write(std::ostream& str) const override;
};

#endif
