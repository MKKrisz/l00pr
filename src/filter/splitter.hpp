#ifndef L00PR_FILTER_SPLITTER
#define L00PR_FILTER_SPLITTER

#include "filter.hpp"

//TODO: this
class Splitter : public Filter {
    std::vector<Filter*> paths;
public:
    Splitter(const Splitter& f) : Filter(f) {
        for(Filter* path : f.paths) {
            paths.emplace_back((Filter*)path->copy());
        }
    }
    Splitter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) : Filter(), paths() {
        str >> expect('(');
        while((str >> skipws).peek() != ')') {
            paths.emplace_back((Filter*)AudioSource::Make(str, srate, MakeFlags::onlyFilters));
        }
        str.get();
        if((str >> skipws).peek() == '{') {
            str.get();
            src = AudioSource::Make(str, srate, flags);
            str >> expect('}');
        }
    }
    
    virtual double filter(double sample, double, double, double) {
        double sum = 0;
        for(Filter* path : paths) {
            path->addSample(sample);
            sum += path->calc();
        }
        return sum;
    }

    AudioSource* copy() { return new Splitter(*this); }
    ~Splitter() {
        for(Filter* f : paths)
            delete f;
    }

    std::string ToString() { return Filter::ToString() + "BlackBox(TM)"; }
    static AudioSource* Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return new Splitter(str, srate, flags);
    }
};

#endif
