#ifndef L00PR_TUNE
#define L00PR_TUNE

#include "../audiosource.h"
#include "../player/noteplayer.hpp"
#include "notestream.hpp"
#include "lane.hpp"

class Tune : public AudioSource {
    std::vector<Lane> lanes;
    std::vector<Generator*> generators;

    double bpm = 60;
    int srate = 48000;
    bool polynote = true;

    double t = 0;
public:
    void setEnv(std::istream& stream);
    void setGen(std::istream& stream);
    void addLane(std::istream& stream);

    double getLen();
    inline double getLaneCount() { return lanes.size(); }
    inline Lane& getLane(size_t id) {
        if(id >= lanes.size())
            throw std::runtime_error("Index out of range for tune lanes");
        return lanes[id];
    }
    inline int getGenCount() { return generators.size(); }
    inline Generator* getGenerator(size_t id) {
        if(id >= generators.size())
            throw std::runtime_error("Index out of range for generators");
        return generators[id];
    }

    inline int getSampleRate() { return srate; }
    
    Tune();
    Tune(NotePlayer&, NoteStream&);
    Tune(Lane& p);

    template <std::ranges::range T>
        requires std::same_as<std::ranges::range_value_t<T>, Lane>
    Tune(T data);
    
    double getSample(double srate);
    ~Tune();
};

std::istream& operator>>(std::istream&, Tune&);

#endif
