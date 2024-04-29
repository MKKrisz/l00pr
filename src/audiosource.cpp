#include "audiosource.hpp"
#include "util.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"

void AudioSource::parse_lb(std::istream& str) {
    str >> skipws;
    if(str.peek() != '{') {
        length_bounds = {};
    }
    str.get();
    str >> skipws;
    double min = 0, max = std::numeric_limits<double>::infinity();
    if(str.peek() != '-') 
        str >> min >> expect('-');
    else str.get();
    if(str.peek() != '}')
        str >> max >> expect('}');
    else str.get();
    if(min > max) throw parse_error(str, "Minimum value cannot be greater than maximum value!");
    length_bounds =  std::make_pair(min, max);
}


const MakeFlags MakeFlags::all = {true, true};
const MakeFlags MakeFlags::onlyFilters = {true, false};
const MakeFlags MakeFlags::onlyGenerators = {false, true};

AudioSource* AudioSource::Make(std::istream& str, const int srate, const MakeFlags& flags) {
    str >> skipws;
    std::string buf;
    char c;
    size_t len = std::max(Filter::LongestName(), Generator::LongestName());
    while(buf.size() < len && str.get(c)) {
        buf += tolower(c);
        if(flags.filters && Filter::ValidName(buf)) {
            return Filter::Make(buf, str, srate);
        }
        if(flags.generators && Generator::ValidName(buf)) {
            return Generator::Make(buf, str, srate, flags);
        }
    }
    throw parse_error(str, "No source with name \"" + buf + "\" exists");
}
