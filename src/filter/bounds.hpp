#pragma once
#include "filter.hpp"
#include "../player/note.hpp"
#include "../player/builtin.hpp"

class BoundsFilter : public Filter {
    double lower = 0;
    double upper = std::numeric_limits<double>::infinity();
public:
    BoundsFilter() : Filter(nullptr) {}
    BoundsFilter(Source* s) : Filter(s) {}
    BoundsFilter(const BoundsFilter& f) : Filter(f), lower(f.lower), upper(f.upper) {}
    BoundsFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) {
        str >> expect('(') >> skipws;
        if (isdigit(str.peek())) {str >> lower;}
        str >> expect(':') >> skipws;
        if (isdigit(str.peek())) {str >> upper;}
        str >> expect(')') >> skipws;
        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
    }
    void addNote(std::unique_ptr<Note> n) {
        if (src == nullptr) { return; }

        PlayableNoteBase* n_casted = dynamic_cast<PlayableNoteBase*>(n.get());

        if (n_casted != nullptr) {
            n_casted->ClampLen(lower, upper);
        }

        src->addNote(std::move(n));
    }

    inline double filter(double sample, double, double, int) { return sample; }

    std::unique_ptr<Source> copy() {return std::make_unique<BoundsFilter>(*this); }

    inline BoundsFilter& operator=(const BoundsFilter&) = default;

    std::string ToString() const {return Filter::ToString() + "Bounds(" + std::to_string(lower) + ":" + std::to_string(upper) + ")";}
    std::string GetNameAndParams() const { return "bounds(" + std::to_string(lower) + ":" + std::to_string(upper) + ")"; }

    static std::unique_ptr<BoundsFilter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<BoundsFilter>(str, srate, flags);
    }
};
