#ifndef L00PR_TUNE
#define L00PR_TUNE

#include "audiosource.h"
#include "noteplayer.hpp"
#include "notestream.hpp"
#include "lane.hpp"

class Tune : public AudioSource {
    std::vector<Lane> lanes;
    std::vector<Generator*> generators;

    float bpm = 60;
    int srate = 48000;
    bool polynote = true;

    double t = 0;
public:
    void setEnv(std::istream& stream);
    void setGen(std::istream& stream);
    void addLane(std::istream& stream);

    float getLen();

    inline int getSampleRate() { return srate; }
    
    Tune();
    Tune(NotePlayer&, NoteStream&);
    Tune(Lane& p);

    template <std::ranges::range T>
        requires std::same_as<std::ranges::range_value_t<T>, Lane>
    Tune(T data);
    
    float getSample(float srate);
    ~Tune();
};

std::istream& operator>>(std::istream&, Tune&);

#endif
