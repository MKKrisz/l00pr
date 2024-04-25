#ifndef L00PR_AUDIOSOURCE_H
#define L00PR_AUDIOSOURCE_H

#include <vector>
#include <istream>

struct MakeFlags {
    bool filters;
    bool generators;

    static MakeFlags all;
};

class AudioSource {
protected:
    std::vector<double> phases;
    double feedback;
    AudioSource(const AudioSource& src) : phases(src.phases), feedback() {}
    AudioSource() : phases(), feedback(0) {}
public:
    virtual inline void addPhase() {
        phases.emplace_back(0);
    }

    virtual inline void removePhase(int id) {
        phases.erase(std::next(phases.begin(), id));
    }

    inline std::vector<double> getPhases() { return phases; }
    inline void setPhases(std::vector<double>& p) { phases = p;}
    
    virtual AudioSource* copy() = 0;

    virtual double operator()(int noteId, double delta, double t, double srate) = 0;
    AudioSource& operator=(const AudioSource&) = default;
    virtual ~AudioSource() {};

    static AudioSource* Make(std::istream&, int = 44100, MakeFlags& = MakeFlags::all);
};

#endif
