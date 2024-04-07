#ifndef L00PR_NOTESTREAM
#define L00PR_NOTESTREAM

#include "../player/note.hpp"

/// A "stream" of notes...
class NoteStream {
    /// Returns the place where t should be in data
    int getId(float t);

    /// Does not work, deprecated, should sort but instead breaks valgrind
    void sort(int l, int h);
    int partition(int l, int h);

    /// Thre Data
    std::vector<std::pair<float, Note>> data;

    /// Beats per minute, used for parsing (so you don't need to count the seconds)
    float bpm = 60;

    /// Enables playing multiple notes at the same time.
    /// This is achieved by changing the parsing format to include a timestamp 
    /// at the beginning
    bool polynote = true;
    
public:
    /// Does not work dont use it
    inline void sort() {sort(0, -1);}

    /// self explanatory getters/setters
    inline void setBpm(float val) { bpm = val; }
    inline float getBpm() const { return bpm;}
    inline void setPolynote(bool val) { polynote = val; }
    inline bool getPolynote() const { return polynote; }

    /// Returns how many seconds it takes to play the stream
    inline float getLen() {
        if(data.size() == 0) return 0;
        std::pair<float, Note> last = data[data.size()-1];
        return last.first + last.second.getLen();
    }

    /// constructors
    inline NoteStream() : data() {}
    inline NoteStream(Note n);
    inline NoteStream(float t, Note n);
    inline NoteStream(std::pair<float, Note> data);
    NoteStream(const NoteStream& s);
    NoteStream(const char* s, int* offset = nullptr);  //This one is deprecated dont use it
 
    /// Adds a note at the appropriate location
    void Add(std::pair<float, Note>& p);

    /// Removes the first float-Note pair from the stream then returns it
    std::pair<float, Note> PopFirst();

    /// Returns all notes that start before t and have not been started yet
    std::vector<Note> GetStartingNotes(float t);
};

/// Parser
std::istream& operator>>(std::istream& stream, NoteStream& ns);

#endif
