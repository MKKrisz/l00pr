#include "set_kwd.hpp"
#include "../util.hpp"
#include "tune.hpp"

void Set::Init() {
    AddMetadata(Set_Metadata("bpm", Set::Bpm));
    AddMetadata(Set_Metadata("samplerate", Set::SampleRate));
    AddMetadata(Set_Metadata("srate", Set::SampleRate));
    AddMetadata(Set_Metadata("poly", Set::Polynote));
    AddMetadata(Set_Metadata("polynote", Set::Polynote));
    AddMetadata(Set_Metadata("nopoly", Set::NoPolynote));
    AddMetadata(Set_Metadata("nopolynote", Set::NoPolynote));
    AddMetadata(Set_Metadata("globalfilter", Set::GlobalFilter));
    AddMetadata(Set_Metadata("gfilter", Set::GlobalFilter));

}

void Set::Bpm(std::istream& str, Tune* t) {
    if((str >> skipws).peek() != ':') {
        //TODO: Make the def. value changeable
        t->bpm(60);
        return;
    }
    double val;
    str.get();
    str >> val;
    t->bpm(val);
}

void Set::SampleRate(std::istream& str, Tune* t) {
    if((str >> skipws).peek() != ':') { 
        t->samplerate(48000);
        return;
    }
    str.get();
    int val;
    str >> val; 
    t->samplerate(val);
}

void Set::Polynote(std::istream&, Tune* t) {
    t->polynote(true);
}

void Set::NoPolynote(std::istream&, Tune* t) {
    t->polynote(false);
}

void Set::GlobalFilter(std::istream& str, Tune* t) {
    if((str>>skipws).peek() != ':') {
        t->globalFilter(nullptr);
        return;
    }
    str.get();
    t->globalFilter((std::unique_ptr<Filter>&&)std::move(AudioSource::Make(str, t->samplerate(), MakeFlags::onlyFilters)));
}
