#include "audiosource.hpp"
#include "util.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"

#include <iostream>

std::string AS_Metadata::ToString() {
    std::string ret = keyword;
    size_t maxKwdLen = std::max(Generator::GetLongestKeywordMeta().keyword.size(), Filter::GetLongestKeywordMeta().keyword.size());
    for(size_t i = 0; i < (maxKwdLen - keyword.size())/8 + 1; i++) {
        ret += '\t';
    }
    ret += (maxKwdLen == keyword.size()?"":"\tSyntax: ") + syntax + "\t" + desc;
    return ret;
}

void AudioSource::parse_lb(std::istream& str) {
    str >> skipws;
    if(str.peek() != '{') {
        length_bounds = {};
        return;
    }
    str.get();
    str >> skipws;
    double min = 0, max = std::numeric_limits<double>::infinity();
    if(str.peek() != '-') 
        str >> min >> expect('-');
    else str.get();
    str >> skipws;
    if(str.peek() != '}')
        str >> max >> expect('}');
    else str.get();
    if(min > max) throw parse_error(str, "Minimum value cannot be greater than maximum value!");
    length_bounds =  std::make_pair(min, max);
}


const MakeFlags MakeFlags::all = {true, true};
const MakeFlags MakeFlags::onlyFilters = {true, false};
const MakeFlags MakeFlags::onlyGenerators = {false, true};

std::unique_ptr<AudioSource> AudioSource::Make(std::istream& str, const int srate, const MakeFlags& flags) {
    auto start = str.tellg();
    std::string gen_except = ""; 
    std::string filter_except = "";
    std::string name = "";

    if((str >> skipws).peek() == ':') {
        str.get();
        char c;
        while(str.get(c) && c != ':') {
            if(isspace(c)) throw parse_error(str, "Named generator names don't allow for whitespaces");
            name += c;
        }
        str >> skipws;
        start = str.tellg();
    }
    if(flags.generators) {
        str.clear();
        str.seekg(start);
        try {
            auto ret = Generator::Parse(str, srate, flags);
            ret->name = name;
            return ret;
        }
        catch(const std::exception& e) {
            gen_except = e.what();
        }
    }
    if(flags.filters) {
        str.clear();
        str.seekg(start);
        try {
            auto ret = Filter::Parse(str, srate, flags);
            ret->name = name;
            return ret;
        }
        catch(const std::exception& e) {
            filter_except = e.what();
        }
    }
    str.clear();
    str.seekg(start);
    throw std::runtime_error(gen_except + '\n' + filter_except);
}


AudioSource* AudioSource::getByName(const std::vector<AudioSource*>& sources, const std::string& name) {
    for(AudioSource* src : sources) {
        if(src->name == name) {
            return src;
        }
    }
    throw std::out_of_range("No audiosource with label " + name);
}

AudioSource* AudioSource::getByName(const std::vector<std::unique_ptr<AudioSource>>& sources, const std::string& name) {
    for(const auto& src : sources) {
        if(src->name == name) {
            return src.get();
        }
    }
    throw std::out_of_range("No audiosource with label " + name);
}
