#include "audiosource.hpp"
#include "util.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"
#include "player/note.hpp"

#include <iostream>

std::string AS_Metadata::ToString() const {
    std::string ret = keyword;
    size_t maxKwdLen = std::max(Generator::GetLongestKeywordMeta().keyword.size(), Filter::GetLongestKeywordMeta().keyword.size());
    for(size_t i = 0; i < (maxKwdLen - keyword.size())/8 + 1; i++) {
        ret += '\t';
    }
    ret += (maxKwdLen == keyword.size()?"":"\tSyntax: ") + syntax + "\t" + desc;
    return ret;
}
std::vector<Source*> Source::getLabeled() {
    if(m_label.empty()) return {};
    return {this};
}

const MakeFlags MakeFlags::all = {true, true};
const MakeFlags MakeFlags::onlyFilters = {true, false};
const MakeFlags MakeFlags::onlyGenerators = {false, true};

std::unique_ptr<Source> Source::Make(std::istream& str, const int srate, const MakeFlags& flags) {
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
            ret->m_label = name;
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
            ret->m_label = name;
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


Source* Source::getByName(const std::vector<Source*>& sources, const std::string& name) {
    for(Source* src : sources) {
        if(src->m_label == name) {
            return src;
        }
    }
    throw std::out_of_range("No audiosource with label " + name);
}

Source* Source::getByName(const std::vector<std::unique_ptr<Source>>& sources, const std::string& name) {
    for(const auto& src : sources) {
        if(src->m_label == name) {
            return src.get();
        }
    }
    throw std::out_of_range("No audiosource with label " + name);
}

SourceRef::SourceRef(const std::string& label, const std::string& err) : Source(), wanted_label(label), wanted_id(0), err_str(err) {}
SourceRef::SourceRef(size_t id, const std::string& err) : Source(), wanted_label(""), wanted_id(id), err_str(err) {}

std::pair<const std::string&, size_t> SourceRef::getWanted() {return std::make_pair(wanted_label, wanted_id);}

void SourceRef::setSource(Source* src) {this->src = src;}
const std::string& SourceRef::getError() {return err_str; }

bool SourceRef::resolved() { return src != nullptr && (labeled() ? src->label() == wanted_label : true ); } // We have no way of checking if it's the correct ID
bool SourceRef::labeled() {return !wanted_label.empty(); }

void SourceRef::assert_resolved() {
     if(src == nullptr) throw std::runtime_error("Unresolved source reference " + wanted_label);
}
void SourceRef::addNote(std::unique_ptr<Note> note) {
#ifndef NDEBUG
    assert_resolved();
#endif
    src->addNote(std::move(note));
}

void SourceRef::addSample(double sample) {
#ifndef NDEBUG
    assert_resolved();
#endif
    src->addSample(sample);
}
std::vector<SourceRef*> SourceRef::getSourceRefs() { return {this}; }

double SourceRef::getFrequencyMultiplier(double t) {
#ifndef NDEBUG
    assert_resolved();
#endif
    return src->getFrequencyMultiplier(t);
}

double SourceRef::getSample(int srate) {
#ifndef NDEBUG
    assert_resolved();
#endif
    return src->getSample(srate);
}

std::unique_ptr<Source> SourceRef::copy() {
#ifndef NDEBUG
    assert_resolved();
#endif
    return src->copy();
}

void SourceRef::operator()(double phase, double t, int srate, double extmul) {
#ifndef NDEBUG
    assert_resolved();
#endif
    (*src)(phase, t, srate, extmul);
}

std::string SourceRef::ToString() const { return ""; }
void SourceRef::Write(std::ostream&) const {}
