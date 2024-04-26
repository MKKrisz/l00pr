#ifndef L00PR_AUDIOSOURCE_H
#define L00PR_AUDIOSOURCE_H

#include <vector>
#include <istream>

struct MakeFlags {
    bool filters;
    bool generators;

    const static MakeFlags all;
    const static MakeFlags onlyFilters;
    const static MakeFlags onlyGenerators;
};

class AudioSource {
protected:
    std::vector<double> phases;
    double feedback;
    double accumulator;
    AudioSource(const AudioSource& src) : phases(src.phases), feedback(), accumulator() {}
    AudioSource() : phases(), feedback(0), accumulator() {}
public:
    virtual void recvFeedback(double val, size_t) {
        feedback = val;
    }
    virtual AudioSource* getBase() {return this;}

    virtual inline void addPhase() {
        phases.emplace_back(0);
    }

    virtual inline void removePhase(int id) {
        phases.erase(std::next(phases.begin(), id));
    }

    inline std::vector<double> getPhases() { return phases; }
    inline void setPhases(std::vector<double>& p) { phases = p;}
    virtual double calc() {
        double ret = accumulator + feedback;
        accumulator = 0;
        return ret;
    }
    virtual AudioSource* copy() = 0;
    
    virtual void operator()(int noteId, double delta, double t, double extmul, double srate) = 0;
    
    AudioSource& operator=(const AudioSource&) = default;
    virtual ~AudioSource() {};

    static AudioSource* Make(std::istream&, const int = 44100, const MakeFlags& = MakeFlags::all);
};

#endif
