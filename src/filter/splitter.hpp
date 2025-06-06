#ifndef L00PR_FILTER_SPLITTER
#define L00PR_FILTER_SPLITTER

#include "filter.hpp"
#include <sstream>

//TODO: this
class Splitter : public Filter {
    std::vector<std::unique_ptr<Filter>> paths;
public:
    Splitter(const Splitter& f) : Filter(f) {
        for(auto& path : f.paths) {
            paths.emplace_back((std::unique_ptr<Filter>&&)std::move(path->copy()));
        }
    }
    Splitter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all) : Filter(), paths() {
        str >> expect('(');
        while((str >> skipws).peek() != ')') {
            paths.emplace_back((std::unique_ptr<Filter>&&)std::move(AudioSource::Make(str, srate, MakeFlags::onlyFilters)));
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
        for(auto& path : paths) {
            path->addSample(sample);
            sum += path->calc();
        }
        return sum;
    }

    std::unique_ptr<AudioSource> copy() { return std::make_unique<Splitter>(*this); }

    std::string ToString() const { return Filter::ToString() + "BlackBox(TM)"; }
    static std::unique_ptr<Splitter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<Splitter>(str, srate, flags);
    }

    std::string GetNameAndParams() const {
        std::stringstream ss {};
        ss << "split(";
        for(const auto& path : paths) {
            ss << std::endl;
            path->Write(ss);
        }
        ss << std::endl << ")";
        return ss.str();
    }
};

#endif
