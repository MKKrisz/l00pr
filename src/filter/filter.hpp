#ifndef L00PR_FILTER
#define L00PR_FILTER

#include "../audiosource.hpp"

/// <summary> 
/// Modifies "incoming" audio samples 
/// General syntax: filter_name(<arguments>) {[src]}
/// The value `src` is another AudioSource. Usually `src` is not strictly as some filters make use of so-called generatorless filter chain (ex.: feedback filters)
/// </summary>
class Filter : public AudioSource {
protected:

    /// <summary> The filter receives its' samples from here (in most cases...) </summary>
    AudioSource* src = nullptr;

    // base cctors
    Filter() : AudioSource() {}
    Filter(AudioSource* src) : AudioSource() {
        if(src != nullptr) 
            this->src = src->copy();
    }
    Filter(const Filter& f) : AudioSource(f), src(f.src == nullptr? nullptr : (f.src)->copy()) {}
public:

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
    virtual void operator()(int noteId, double delta, double t, double srate, double extmul) override;
    
    // dtor
    inline virtual ~Filter() { delete src; }

    // copy assignment operator
    Filter& operator=(const Filter& f) {
        if(this == &f) return *this;
        AudioSource::operator=(f);
        src = f.src != nullptr? f.src->copy() : nullptr;
        return *this;
    }

    /// <summary> Checks if `name` is  a valid Filter name </summary>
    static bool ValidName(const std::string& name);

    // I want this to be constexpr, but it cannot be constexpr
    /// <summary> Returns the length of the longest filter name</summary>
    static size_t LongestName();

    /// <summary> Creates a filter of type `name` </summary>
    /// <returns> A pointer to the created filter </returns>
    static Filter* Make(std::string& name, std::istream& str, const int srate = 44100, const MakeFlags& = MakeFlags::all);
};

#endif
