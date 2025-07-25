#ifndef L00PR_FILTER
#define L00PR_FILTER

#include "../audiosource.hpp"

class Filter;

struct Filter_Metadata : public Metadata<std::unique_ptr<Source>, const int, const MakeFlags&>{
public:
    std::string syntax;
    std::string desc;
    Filter_Metadata(
        const char* kw,
        std::function<
            std::unique_ptr<Source>(
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
/// The value `src` is another Source. Usually `src` is not strictly as some filters make use of so-called generatorless filter chain (ex.: feedback filters)
/// </summary>
class Filter : public Source, public Parseable<std::unique_ptr<Source>, Filter_Metadata, const int, const MakeFlags&> {
protected:

    /// <summary> The filter receives its' samples from here (in most cases...) </summary>
    std::unique_ptr<Source> src = nullptr;

    // base cctors
    Filter() : Source() {}
    Filter(Source* src) : Source() {
        if(src != nullptr) 
            this->src = src->copy();
    }
    Filter(const Filter& f) : Source(f), src(f.src == nullptr? nullptr : (f.src)->copy()) {}
public:
    static void Init();

    void addNote(std::unique_ptr<Note> note) override;

    /// <summary> Function for  handling feedback samples </summary>
    virtual void recvFeedback(double val, size_t depth) override {
        if(depth > 0 && src != nullptr) {
            src->recvFeedback(val, depth-1);
            return;
        }
        m_feedback = val;
    }

    virtual void addSample(double sample) override {
        if(src != nullptr) {
            src->addSample(sample);
            return;
        }
        Source::addSample(sample);
    }

    /// <summary> Unused function, returns the base generator of a filter chain </summary>
    virtual Source* getBase() override {
        return src == nullptr? nullptr : src->getBase();
    }
    std::vector<SourceRef*> getSourceRefs() override {
        return src==nullptr ? std::vector<SourceRef*>() : src->getSourceRefs();
    }
    std::vector<Source*> getLabeled() override {
        if(src == nullptr) { return Source::getLabeled(); }
        auto labels = src->getLabeled();
        auto self = Source::getLabeled();
        labels.insert(labels.end(), self.begin(), self.end());
        return labels;
    }

    /// <summary> Function that gets called by calc() to filter the sample. </summary>
    virtual double filter(double sample, double delta, double t, int srate) = 0;

    /// <summary> Filters the sample given by `src` </summary>
    /// <returns> The filtered sample </summary>
    virtual double getSample(int samplerate) override;

    virtual double getSingleSample(double, double, int) override;

    /// <summary> Propagates the call down to the base generator </summary>
    virtual void operator()(double phase, double t, int srate, double note_amplitude) override;
    
    // dtor
    inline virtual ~Filter() {}
    virtual std::unique_ptr<Source> copy() override = 0;

    // copy assignment operator
    Filter& operator=(const Filter& f) {
        if(this == &f) return *this;
        Source::operator=(f);
        src = f.src != nullptr? f.src->copy() : nullptr;
        return *this;
    }

    virtual std::string ToString() const override {return (src == nullptr? "" : src->ToString() + " -> "); }

    virtual std::string GetNameAndParams() const = 0;
    void Write(std::ostream&) const override; 

    static std::string getFormattedMetadata();
};

#endif
