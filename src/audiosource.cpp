#include "audiosource.hpp"
#include "util.hpp"
#include "generator/builtin.hpp"
#include "filter/builtin.hpp"

MakeFlags MakeFlags::all = {true, true};

AudioSource* AudioSource::Make(std::istream& str, int srate, MakeFlags& flags) {
    str >> skipws;
    std::string buf;
    char c;
    while(str.get(c)) {
        buf += tolower(c);
        if(flags.filters && buf.size() <= Filter::LongestName()) {
            if(Filter::ValidName(buf)) {
                return Filter::Make(buf, str, srate);
            }
        }
        else break;
        if(flags.generators && buf.size() <= Generator::LongestName()) {
            if(Generator::ValidName(buf)) {
                return Generator::Make(buf, str, srate, flags);
            }
        }
        else break;
    }
    throw parse_error(str, "No source with name \"" + buf + "\" exists");
}
