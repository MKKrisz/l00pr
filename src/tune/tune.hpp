#ifndef L00PR_TUNE
#define L00PR_TUNE

#include "../audiosource.hpp"
#include "../filter/filter.hpp"
#include "../filter/dummy.hpp"
#include "../player/noteplayer.hpp"
#include "notestream.hpp"
#include "lane.hpp"
#include <memory>

class Tune;

class Tkwd_Metadata : public Metadata<void, Tune*> {
public:
    Tkwd_Metadata(const char* str, std::function<void(std::istream&, Tune*)> func) 
        : Metadata(str, func) {}
    Tkwd_Metadata(const Tkwd_Metadata& m) : Metadata(m) {/*nothing*/}

    Tkwd_Metadata& operator=(const Tkwd_Metadata& m) = default;
};

/// <summary> Stores a piece of sound data to be played </summary>
class Tune : public Parseable<void, Tkwd_Metadata, Tune*>{
public:
    static void Init();
    static void SetGen(std::istream&, Tune*);
    static void AddLane(std::istream& stream, Tune*);

    /// <summary> Parses the generator keyword </summary>
    void setGen(std::istream& stream);

    /// <summary> Parses the player keyword </summary>
    void addLane(std::istream& stream);

    void   bpm(double bpm) { this->m_bpm = bpm; }
    double bpm()           { return m_bpm; }

    /// <summary> Returns the sample rate value
    void samplerate(int sr) { m_srate = sr; }
    int  samplerate()       { return m_srate; }

    void polynote(bool val) { m_polynote = val; }
    bool polynote()         { return m_polynote; }

    void globalFilter(std::unique_ptr<Filter> src) {
        p_globalFilter = (src.get() != nullptr ? std::move(src) : std::make_unique<DummyFilter>());
    }
    AudioSource* globalFilter() { return p_globalFilter.get(); }

    /// <summary> Gets the time it takes to play the tune </summary>
    double getLen();
    bool isNotePlaying();

    /// <summary> Checks if the elapsed time since start is greater than the time required to play the tune </summary>
    bool isComplete() { return t > getLen();}

    /// <summary> Gets the number of lanes </summary>
    double laneCount() const { return m_lanes.size(); }

    /// <summary> Returns a reference to the Lane at index `id` </summary>
    Lane& lane(size_t id) {
        if(id >= m_lanes.size())
            throw std::runtime_error("Index out of range for tune lanes");
        return m_lanes[id];
    }

    /// <summary> Gets the number of generators </summary>
    int sourceCount() const { return p_sources.size(); }

    /// <summary> Returns a pointer to the generator at index `id` </summary>
    AudioSource* getSource(size_t id) {
        if(id >= p_sources.size())
            throw std::runtime_error("Index out of range for generators");
        return p_sources[id].get();
    }
    AudioSource* getSourceByName(std::string name);

    Tune();
    Tune(NotePlayer&, NoteStream&);
    Tune(Lane& p);
    Tune(const Tune& t);

    template <std::ranges::range T>
        requires std::same_as<std::ranges::range_value_t<T>, Lane>
    Tune(T data);
    
    /// <summary> Calculates the next sample value </summary>
    [[nodiscard]] double getSample(double srate, bool print = true) ;
    void Write(std::ostream&) const override;

    // dtor
    ~Tune() {}

private:

    /// <summary> Stores NotePlayer-NoteStream pairs
    std::vector<Lane> m_lanes;

    /// <summary> Sources specified using the "generator" keyword end up in this array </summary>
    std::vector<std::unique_ptr<AudioSource>> p_sources;

    std::unique_ptr<Filter> p_globalFilter = std::make_unique<DummyFilter>();

    /// <summary> Beats per minute value, used to make calculating lengths and times a little easier </summary>
    double m_bpm = 60;

    /// <summary> Sample rate. Used for setting up specific types of sources, and many more things! </summary>
    int m_srate = 48000;

    /// <summary> A boolean that specifies wether multiple notes playing at the same time is allowed or not </summary>
    bool m_polynote = true;

    /// <summary> A value indicating how much time has elapsed since the start of playing </summary>
    double t = 0;

    std::vector<AudioSource*> getSources();

};

/// <summary> Parser for Tunes </summary>
/// <exception cref="parse_error"> On parsing failure </exception>
std::istream& operator>>(std::istream&, Tune&);

#endif
