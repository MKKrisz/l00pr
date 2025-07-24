#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "noteplayer.hpp"
#include "../parseable_base.hpp"
#include "../string_convertible.hpp"

class Note;
class Tune;

class Note_Metadata : public Metadata<std::unique_ptr<Note>, Tune*, double, bool, int> {
public:
    std::string syntax;
    std::string desc;
    Note_Metadata(const char* kw, std::function<std::unique_ptr<Note>(std::istream&, Tune*, double, bool, int)> func, const char* syn, const char* desc) 
        : Metadata(kw, func), syntax(syn), desc(desc) {};
    Note_Metadata(const Note_Metadata& meta) : Metadata(meta), syntax(meta.syntax), desc(meta.desc) {}
    std::string ToString() const override;

    Note_Metadata& operator=(const Note_Metadata& m) = default;
};

class Note : public StringConvertible, public Parseable<std::unique_ptr<Note>, Note_Metadata, Tune*, double, bool, int> {
protected:
    Note() : note_id(note_id_ctr++) {}
    Note(const Note& n) : note_id(n.note_id) {}
public:
    virtual void AddToPlayer(NotePlayer&) {/*nothing*/};
    virtual void AddToSource(Source*) {/*nothing*/}
    virtual void AddSample(Source* p, int srate) = 0;
    virtual void RemoveFromSource(Source*) {/*nothing*/}

    virtual bool IsComplete() const = 0;
    virtual double GetLen() const = 0;
    virtual std::unique_ptr<Note> copy() const = 0;
    virtual std::string ToString() const {return "[Note:" + std::to_string(GetLen()) + "]";}
    virtual ~Note() {}

    bool operator==(const Note& other) { return note_id == other.note_id; }

    static void Init();

    static std::unique_ptr<Note> Make(std::istream&, Tune*, double, bool, int);

private:
    static uint32_t note_id_ctr;
    uint32_t note_id;
};

#endif
