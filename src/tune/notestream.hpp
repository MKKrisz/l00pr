#ifndef L00PR_NOTESTREAM
#define L00PR_NOTESTREAM

#include "../player/note.hpp"
#include "../util.hpp"

class Loop;
class RandomNote;
class Tune;

/// <summary> Class that stores and retreives note data
class NoteStream : public virtual Writeable{
protected:
    /// Returns the place where t should be in playable
    inline size_t getId(double t) { return bSearch(notes, t); }

    /// The Data
    std::vector<std::pair<double, std::unique_ptr<Note>>> notes;

    /// <summary> Beats per minute calue, used for parsing (so you don't need to count the seconds) </summary>
    double bpm = 60;

    /// <summary> Enables playing multiple notes at the same time. </summary>
    /// <remarks>
    /// This is achieved by changing the parsing format to include a timestamp 
    /// at the beginning
    /// </remarks>
    bool polynote = true;

    /// <summary> Sample rate. Used to properly calculate filters and other things</summary>
    int srate = 48000;

    // <summary> Length of notestream. If less than zero, it has not been cached yet </summary>
    double len = -1;
    // <summary> The timestamp when the last note starts. </summary>
    double lastNoteTs = -1;
    // <summary> 
    size_t m_next_note_idx = 0;

public:
    void calculateLen();

    /// self explanatory getters/setters
    inline void setBpm(double val) { bpm = val; }
    inline double getBpm() const { return bpm;}

    inline void setPolynote(bool val) { polynote = val; }
    inline bool getPolynote() const { return polynote; }

    inline void setSampleRate(double val) { srate = val; }
    inline int getSampleRate() const { return srate; }

    /// <summary> Caluclates how many seconds it takes to play the stream. This value is cached, if you modify the stream in any way, please call invalidateLen() </summary>
    double getLen() const;
    void invalidateLen() {len = -1; lastNoteTs = -1;}
    inline size_t size() const {return notes.size();}

    /// <summary> Returns a reference to the note at index `id` </summary>
    inline Note* getNote(size_t id) {
        if(id >= notes.size())
            throw std::out_of_range("NoteStream.playable");
        return notes[id].second.get();
    }
    inline const Note* getNote(size_t id) const {
        if(id >= notes.size())
            throw std::out_of_range("NoteStream.playable");
        return notes[id].second.get();
    }

    /// constructors
    inline NoteStream() : notes() {calculateLen();}
    NoteStream(Note* n);
    NoteStream(double t, Note* n);
    NoteStream(std::pair<double, Note*> note);
    NoteStream(const NoteStream& s);
    NoteStream(std::istream& str, Tune* tune, double bpm, bool polynote, int srate);

    /// <summary> Adds a note to the structure at the specified timestamp </summary>
    void Add(std::pair<double, std::unique_ptr<Note>> p) { ordered_add(notes, p); }

    /// <summary> Returns all notes that start before t and have not been started yet </summary>
    virtual std::vector<std::unique_ptr<Note>> GetStartingNotes(double t);

    NoteStream& operator=(const NoteStream& s){
        if(&s == this) return *this;
        len = s.len;
        lastNoteTs = s.lastNoteTs;
        srate = s.srate;
        bpm = s.bpm;
        polynote = s.polynote;
        for(size_t i = 0; i < s.notes.size(); i++) {
            notes.emplace_back(std::make_pair(s.notes[i].first, s.notes[i].second->copy()));
        }
        return *this;
    }

    ~NoteStream() {}

    void Write(std::ostream& str) const;
};


#endif
