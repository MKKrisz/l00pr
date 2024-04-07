#ifndef L00PR_NOTESTREAM
#define L00PR_NOTESTREAM

#include "../player/note.hpp"
#include "../player/setnote.hpp"

/// A "stream" of notes...
class NoteStream {
    /// Returns the place where t should be in playable
    int getId(float t);
    int getSetterId(float t);

    /// Thre Data
    /// Design decisions:
    ///     This needs to be performant. That can be easily achieved by placing
    ///     the notes close to eachother. This way of implementing achieves that.
    ///     On the other hand this would be way more pleasing if 
    std::vector<std::pair<float, Note>> playable;
    std::vector<std::pair<float, SetterNote>> setter;

    /// Beats per minute, used for parsing (so you don't need to count the seconds)
    float bpm = 60;

    /// Enables playing multiple notes at the same time.
    /// This is achieved by changing the parsing format to include a timestamp 
    /// at the beginning
    bool polynote = true;
    
public:
    /// self explanatory getters/setters
    inline void setBpm(float val) { bpm = val; }
    inline float getBpm() const { return bpm;}
    inline void setPolynote(bool val) { polynote = val; }
    inline bool getPolynote() const { return polynote; }

    /// Returns how many seconds it takes to play the stream
    inline float getLen() {
        if(playable.size() == 0) return 0;
        std::pair<float, Note> last = playable[playable.size()-1];
        return last.first + last.second.getLen();
    }

    inline float getPlayableSize() { return playable.size(); }
    inline float getSetterSize() { return setter.size(); }

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
    inline NoteStream() : playable(), setter() {}
    inline NoteStream(Note n);
    inline NoteStream(float t, Note n);
    inline NoteStream(std::pair<float, Note> playable);
    NoteStream(const NoteStream& s);
 
    /// Adds a note at the appropriate location
    void Add(std::pair<float, Note> p);
    void Add(std::pair<float, SetterNote> p);

    /// Removes the first float-Note pair from the stream then returns it
    std::pair<float, Note> PopFirst();

    /// Returns all notes that start before t and have not been started yet
    std::vector<Note> GetStartingPlayableNotes(float t);
    std::vector<SetterNote> GetStartingSetterNotes(float t);
};

/// Parser
std::istream& operator>>(std::istream& stream, NoteStream& ns);

#endif
