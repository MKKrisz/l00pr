#ifndef L00PR_KEYWORD_SET
#define L00PR_KEYWORD_SET

#include "../parseable_base.hpp"

class Tune;
class Set_Metadata : public Metadata<void, Tune*> {
public:
    Set_Metadata(const char* kwd, std::function<void(std::istream&, Tune*)> func) 
        : Metadata(kwd, func) {}
};

//TODO: Make this not just modify a random class's data...
class Set : public Parseable<void, Set_Metadata, Tune*> {
public:
    static void Init();
    static void Bpm(std::istream& str, Tune* t);
    static void SampleRate(std::istream& str, Tune*);
    static void Polynote(std::istream& str, Tune*);
    static void NoPolynote(std::istream& str, Tune*);
    static void GlobalFilter(std::istream& str, Tune*);
};

#endif
