#ifndef L00PR_FILTER
#define L00PR_FILTER

#include "../audiosource.hpp"

class Filter;

struct Filter_Metadata : public Metadata<std::unique_ptr<AudioSource>, const int, const MakeFlags&>{
public:
    std::string syntax;
    std::string desc;
    Filter_Metadata(
        const char* kw,
        std::function<
            std::unique_ptr<AudioSource>(
                std::istream&,
                const int,
                const MakeFlags&)> func,
        const char* syn,
        const char* desc)
        : Metadata(kw, func), syntax(syn), desc(desc) {};
    Filter_Metadata(const Filter_Metadata& meta) : Metadata(meta), syntax(meta.syntax), desc(meta.desc) {}
    std::string ToString() const override;

    Filter_Metadata& operator=(const Filter_Metadata& m) = default;
};

/// <summary> 
/// Modifies "incoming" audio samples 
/// General syntax: filter_name(<arguments>) {[src]}
/// The value `src` is another AudioSource. Usually `src` is not strictly as some filters make use of so-called generatorless filter chain (ex.: feedback filters)
/// </summary>
class Filter : public AudioSource, public Parseable<std::unique_ptr<AudioSource>, Filter_Metadata, const int, const MakeFlags&> {
protected:

    /// <summary> The filter receives its' samples from here (in most cases...) </summary>
    std::unique_ptr<AudioSource> src = nullptr;

    // base cctors
    Filter() : AudioSource() {}
    Filter(AudioSource* src) : AudioSource() {
        if(src != nullptr) 
            this->src = src->copy();
    }
    Filter(const Filter& f) : AudioSource(f), src(f.src == nullptr? nullptr : (f.src)->copy()) {}
public:
    static void Init();

    /// <summary> Function for  handling feedback samples </summary>
    virtual void recvFeedback(double val, size_t depth) override {
        if(depth > 0 && src != nullptr) {
            src->recvFeedback(val, depth-1);
            return;
        }
        feedback = val;
    }

    virtual std::optional<std::pair<double, double>> getLengthBounds() override {
        if(src == nullptr) return {};
        return src->getLengthBounds();
    }

    virtual void addSample(double sample) override {
        if(src != nullptr) {
            src->addSample(sample);
            return;
        }
        AudioSource::addSample(sample);
    }

    /// <summary> Unused function, returns the base generator of a filter chain </summary>
    virtual AudioSource* getBase() override {
        return src == nullptr? nullptr : src->getBase();
    }

    /// <summary> Adds a phase to the filter chain's base, since filters don't use phases </summary>
    inline void addPhase() override { src->addPhase(); }

    /// <summary> Removes the phase at id from the base of the filter chain </summary>
    inline void removePhase(int id) override { src->removePhase(id);}

    /// <summary> Function that gets called by calc() to filter the sample. </summary>
    virtual double filter(double sample, double delta, double t, double srate) = 0;

    /// <summary> Filters the sample given by `src` </summary>
    /// <returns> The filtered sample </summary>
    virtual double calc() override;

    /// <summary> Propagates the call down to the base generator </summary>
    virtual void operator()(size_t noteId, double delta, double t, double srate, double extmul) override;
    
    // dtor
    inline virtual ~Filter() {}
    virtual std::unique_ptr<AudioSource> copy() override = 0;

    // copy assignment operator
    Filter& operator=(const Filter& f) {
        if(this == &f) return *this;
        AudioSource::operator=(f);
        src = f.src != nullptr? f.src->copy() : nullptr;
        return *this;
    }

    virtual std::string ToString() const override {return (src == nullptr? "" : src->ToString() + " -> "); }

    virtual std::string GetNameAndParams() const = 0;
    void Write(std::ostream&) const override; 

    static std::string getFormattedMetadata();
};

#endif
