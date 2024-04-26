#include "filter.hpp"
#include "../exceptions/parse_error.hpp"
#include "builtin.hpp"


const std::vector<std::string> filterNames = {"pass", "delay", "gain", "feedback"};

void Filter::operator()(int noteId, double delta, double t, double srate, double extmul) {
    (*src)(noteId, delta, t, srate, extmul);
}

double Filter::calc() {
    return filter(src->calc(), 0, 0, 0);
}

bool Filter::ValidName(const std::string& name) {
    for(const std::string& fn : filterNames) {
        if(name == fn) return true;
    }
    return false;
}

size_t Filter::LongestName() {
    size_t l = 0;
    for(const std::string& name : filterNames) {
        if(name.size() > l) l = name.size();
    }
    return l;
}

Filter* Filter::Make(std::string& name, std::istream& str, const int srate, const MakeFlags& flags) {
    if(name == "pass")     return new PassFilter(str, srate, flags); 
    if(name == "delay")    return new DelayFilter(str, srate, flags); 
    if(name == "gain")     return new GainFilter(str, srate, flags);
    if(name == "feedback") return new Feedback(str, srate, flags);
    throw parse_error(str, "No filter with name \"" + name + "\" exists");
}
