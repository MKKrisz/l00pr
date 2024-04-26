#ifndef L00PR_FILTER
#define L00PR_FILTER

#include "../audiosource.hpp"

class Filter : public AudioSource {
protected:
    AudioSource* src = nullptr;

    Filter() : AudioSource() {}
    Filter(AudioSource* src) : AudioSource() {
        if(src != nullptr) 
            this->src = src->copy();
    }
    Filter(const Filter& f) : AudioSource(f), src(f.src == nullptr? nullptr : (f.src)->copy()) {}
public:
    virtual void recvFeedback(double val, size_t depth) override {
        if(depth > 0) {
            src->recvFeedback(val, depth-1);
            return;
        }
        feedback = val;
    }
    virtual AudioSource* getBase() override {
        return src == nullptr? nullptr : src->getBase();
    }

    inline void addPhase() override { src->addPhase(); }
    inline void removePhase(int id) override { src->removePhase(id);}

    virtual double filter(double sample, double delta, double t, double srate) = 0;
    virtual double calc() override;
    virtual void operator()(int noteId, double delta, double t, double srate, double extmul) override;

    
    inline virtual ~Filter() { delete src; }

    Filter& operator=(const Filter& f) {
        if(this == &f) return *this;
        AudioSource::operator=(f);
        src = f.src != nullptr? f.src->copy() : nullptr;
        return *this;
    }

    static bool ValidName(const std::string& name);

    // I want this to be constexpr, but it cannot be constexpr
    static size_t LongestName();
    static Filter* Make(std::string& name, std::istream& str, const int srate = 44100, const MakeFlags& = MakeFlags::all);
};

#endif
