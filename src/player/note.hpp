#ifndef L00PR_NOTE
#define L00PR_NOTE

#include "noteplayer.hpp"
#include "../string_convertible.hpp"

class Note : public StringConvertible{
    public:
    virtual void AddToPlayer(NotePlayer& p) = 0;
    virtual void AddSample(NotePlayer& p, size_t index, int srate) = 0;
    virtual bool IsComplete() = 0;
    virtual double GetLen() = 0;
    virtual Note* copy() = 0;
    virtual std::string ToString() {return "[Note:" + std::to_string(GetLen()) + "]";}
    virtual ~Note() {}
};

#endif
