#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "noteplayer.hpp"
#include "../parseable_base.hpp"
#include "../string_convertible.hpp"

class Note;

class Note_Metadata : public Metadata<Note*, const std::vector<AudioSource*>&, double, bool, int> {
public:
    std::string syntax;
    std::string desc;
    Note_Metadata(const char* kw, std::function<Note*(std::istream&, const std::vector<AudioSource*>&, double, bool, int)> func, const char* syn, const char* desc) 
        : Metadata(kw, func), syntax(syn), desc(desc) {};
    Note_Metadata(const Note_Metadata& meta) : Metadata(meta), syntax(meta.syntax), desc(meta.desc) {}
    std::string ToString() const override;

    Note_Metadata& operator=(const Note_Metadata& m) = default;
};

class Note : public StringConvertible, public Parseable<Note*, Note_Metadata, const std::vector<AudioSource*>&, double, bool, int> {
public:
    virtual void AddToPlayer(NotePlayer& p) = 0;
    virtual void AddSample(NotePlayer& p, size_t index, int srate) = 0;
    virtual bool IsComplete() const = 0;
    virtual double GetLen() const = 0;
    virtual Note* copy() const = 0;
    virtual std::string ToString() const {return "[Note:" + std::to_string(GetLen()) + "]";}
    virtual ~Note() {}

    static void Init();

    static Note* Make(std::istream&, const std::vector<AudioSource*>&, double, bool, int);

private:
    static std::function<Note*(std::istream&, const std::vector<AudioSource*>&, double, bool, int)> default_note_fun;
};

#endif
