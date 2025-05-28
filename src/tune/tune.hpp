#ifndef L00PR_TUNE
#define L00PR_TUNE

#include "../audiosource.hpp"
#include "../filter/filter.hpp"
#include "../filter/dummy.hpp"
#include "../player/noteplayer.hpp"
#include "notestream.hpp"
#include "lane.hpp"

class Tune;

class Tkwd_Metadata : public Metadata<void, Tune*> {
public:
    Tkwd_Metadata(const char* str, std::function<void(std::istream&, Tune*)> func) 
        : Metadata(str, func) {}
};

/// <summary> Stores a piece of sound data to be played </summary>
class Tune : public Parseable<void, Tkwd_Metadata, Tune*>{

    /// <summary> Stores NotePlayer-NoteStream pairs
    std::vector<Lane> lanes;

    /// <summary> Sources specified using the "generator" keyword end up in this array </summary>
    std::vector<AudioSource*> sources;

    Filter* globalFilter = new DummyFilter();

    /// <summary> Beats per minute value, used to make calculating lengths and times a little easier </summary>
    double bpm = 60;

    /// <summary> Sample rate. Used for setting up specific types of sources, and many more things! </summary>
    int srate = 48000;

    /// <summary> A boolean that specifies wether multiple notes playing at the same time is allowed or not </summary>
    bool polynote = true;

    /// <summary> A value indicating how much time has elapsed since the start of playing </summary>
    double t = 0;

    AudioSource* getSourceByName(std::string name);
public:
    static void Init();

    inline void setBpm(double bpm) { this->bpm = bpm; }

    /// <summary> Parses the generator keyword </summary>
    void setGen(std::istream& stream);
    static void SetGen(std::istream&, Tune*);

    /// <summary> Parses the player keyword </summary>
    void addLane(std::istream& stream);
    static void AddLane(std::istream& stream, Tune*);

    /// <summary> Returns the sample rate value
    inline void setSampleRate(int sr) {srate = sr;}
    inline int getSampleRate() { return srate; }

    inline void setPoly(bool val) {polynote = val;}
    inline void setGlobalFilter(Filter* src) {
        delete globalFilter;
        globalFilter = (src == nullptr ? new DummyFilter() : src->copy());
    }

    /// <summary> Gets the time it takes to play the tune </summary>
    double getLen();
    bool noteIsPlaying();

    /// <summary> Checks if the elapsed time since start is greater than the time required to play the tunee </summary>
    inline bool isComplete() { return t > getLen();}

    /// <summary> Gets the number of lanes </summary>
    inline double getLaneCount() const { return lanes.size(); }

    /// <summary> Returns a reference to the Lane at index `id` </summary>
    inline Lane& getLane(size_t id) {
        if(id >= lanes.size())
            throw std::runtime_error("Index out of range for tune lanes");
        return lanes[id];
    }

    /// <summary> Gets the number of generators </summary>
    inline int getGenCount() const { return sources.size(); }

    /// <summary> Returns a pointer to the generator at index `id` </summary>
    inline AudioSource* getGenerator(size_t id) {
        if(id >= sources.size())
            throw std::runtime_error("Index out of range for generators");
        return sources[id];
    }
 
    // cctors
    Tune();
    Tune(NotePlayer&, NoteStream&);
    Tune(Lane& p);
    Tune(const Tune& t);

    template <std::ranges::range T>
        requires std::same_as<std::ranges::range_value_t<T>, Lane>
    Tune(T data);
    
    /// <summary> Calculates the next sample value </summary>
    double getSample(double srate, bool print = true);

    // dtor
    ~Tune();
};

/// <summary> Parser for Tunes </summary>
/// <exception cref="parse_error"> On parsing failure </exception>
std::istream& operator>>(std::istream&, Tune&);

#endif
