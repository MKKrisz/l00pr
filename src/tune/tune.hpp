#ifndef L00PR_TUNE
#define L00PR_TUNE

#include "../audiosource.hpp"
#include "../player/noteplayer.hpp"
#include "notestream.hpp"
#include "lane.hpp"

class Tune {
    std::vector<Lane> lanes;
    std::vector<AudioSource*> sources;

    double bpm = 60;
    int srate = 48000;
    bool polynote = true;

    double t = 0;
public:
    void setEnv(std::istream& stream);
    void setGen(std::istream& stream, int srate = 44100);
    void addLane(std::istream& stream, int srate = 44100);

    inline int getSampleRate() { return srate; }

    double getLen() const;
    inline bool isComplete() const { return t > getLen();}
    inline double getLaneCount() const { return lanes.size(); }
    inline Lane& getLane(size_t id) {
        if(id >= lanes.size())
            throw std::runtime_error("Index out of range for tune lanes");
        return lanes[id];
    }
    inline int getGenCount() const { return sources.size(); }
    inline AudioSource* getGenerator(size_t id) {
        if(id >= sources.size())
            throw std::runtime_error("Index out of range for generators");
        return sources[id];
    }
 
    Tune();
    Tune(NotePlayer&, NoteStream&);
    Tune(Lane& p);
    Tune(const Tune& t);

    template <std::ranges::range T>
        requires std::same_as<std::ranges::range_value_t<T>, Lane>
    Tune(T data);
    
    double getSample(double srate);
    double discardSample(double srate);
    ~Tune();
};

std::istream& operator>>(std::istream&, Tune&);

#endif
