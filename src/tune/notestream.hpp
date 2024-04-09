#ifndef L00PR_NOTESTREAM
#define L00PR_NOTESTREAM

#include "../player/note.hpp"
#include "../player/setnote.hpp"
#include "../util.hpp"

class Loop;

/// A "stream" of notes...
class NoteStream {
protected:
    /// Returns the place where t should be in playable
    inline int getId(double t) { return bSearch(playable, t); }
    inline int getSetterId(double t) { return bSearch(setter, t); }
    inline int getLoopId(double t) { return bSearch(loops, t); }

    /// Thre Data
    /// Design decisions:
    ///     This needs to be performant. That can be easily achieved by placing
    ///     the notes close to eachother. This way of implementing achieves that.
    ///     On the other hand this would be way more pleasing if 
    std::vector<std::pair<double, Note>> playable;
    std::vector<std::pair<double, SetterNote>> setter;
    std::vector<std::pair<double, NoteStream>> loops;

    /// Beats per minute, used for parsing (so you don't need to count the seconds)
    double bpm = 60;

    double len = -1;
    double lastNoteTs = -1;

    /// Enables playing multiple notes at the same time.
    /// This is achieved by changing the parsing format to include a timestamp 
    /// at the beginning
    bool polynote = true;
    
public:
    /// self explanatory getters/setters
    inline void setBpm(double val) { bpm = val; }
    inline double getBpm() const { return bpm;}
    inline void setPolynote(bool val) { polynote = val; }
    inline bool getPolynote() const { return polynote; }

    /// Returns how many seconds it takes to play the stream
    inline double getLen() {
        if(!loops.empty()) return std::numeric_limits<double>::infinity();
        //if(playable.empty()) return 0;
        if (len < 0) {
            std::pair<double, Note> last = playable[playable.size()-1];
            lastNoteTs = last.first;
            len = last.first + last.second.getLen();
        }
        if(playable.empty()) return 0;
        return len;
    }

    inline double getPlayableSize() { return playable.size(); }
    inline double getSetterSize() { return setter.size(); }

    inline SetterNote& getSetterNote(int id) {
        if(id < 0 || id >= setter.size())
            throw std::runtime_error("Index out of range for generator setter notes");
        return setter[id].second;
    }

    inline Note& getPlayableNote(int id) {
        if(id < 0 || id >= playable.size())
            throw std::runtime_error("Index out of range for playable notes");
        return playable[id].second;
    }

    /// constructors
    inline NoteStream() : playable(), setter(), loops() {}
    inline NoteStream(Note n);
    inline NoteStream(double t, Note n);
    inline NoteStream(std::pair<double, Note> playable);
    NoteStream(const NoteStream& s);
 
    /// Adds a note at the appropriate location
    void Add(std::pair<double, Note> p) { ordered_add(playable, p); }
    void Add(std::pair<double, SetterNote> p) { ordered_add(setter, p); }
    void Add(std::pair<double, Loop> p);

    /// Returns all notes that start before t and have not been started yet
    virtual std::vector<Note> GetStartingPlayableNotes(double t);
    virtual std::vector<SetterNote> GetStartingSetterNotes(double t);
};

/// Parser
std::istream& operator>>(std::istream& stream, NoteStream& ns);

#endif
