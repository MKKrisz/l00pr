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
    std::string ToString() override;
};

class Note : public StringConvertible, public Parseable<Note*, Note_Metadata, const std::vector<AudioSource*>&, double, bool, int> {
public:
    virtual void AddToPlayer(NotePlayer& p) = 0;
    virtual void AddSample(NotePlayer& p, size_t index, int srate) = 0;
    virtual bool IsComplete() = 0;
    virtual double GetLen() = 0;
    virtual Note* copy() = 0;
    virtual std::string ToString() {return "[Note:" + std::to_string(GetLen()) + "]";}
    virtual ~Note() {}

    static void Init();

    static Note* Make(std::istream&, const std::vector<AudioSource*>&, double, bool, int);

private:
    static Note_Metadata default_note;
};

#endif
