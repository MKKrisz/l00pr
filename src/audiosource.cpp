#include "audiosource.hpp"
#include "util.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"

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
