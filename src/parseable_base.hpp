#ifndef L00PR_PARSEABLE
#define L00PR_PARSEABLE

#include <string>
#include <vector>
#include <functional>
#include <cstdarg>
#include <istream>

#include "exceptions/parse_error.hpp"
#include "util.hpp"
#include "string_convertible.hpp"

template <typename T, typename ... Args>
class Metadata : StringConvertible {
protected:
    Metadata(const char* kw, std::function<T(std::istream&, Args...)> fn) : keyword(kw), create_func(fn) {}
    Metadata(const Metadata& m) : keyword(m.keyword), create_func(m.create_func) {}
public:
    std::string keyword;
    std::function<T(std::istream&, Args...)> create_func;
    std::string ToString() { return keyword; }
};

///<summary> Base class for all classes that 
template <typename T, typename Metadata_type, typename ... Args>
class Parseable {
protected:
    static std::vector<Metadata_type> meta;
public:
    static void AddMetadata(const Metadata_type& m) { meta.emplace_back(m); }

    static size_t GetKeywordCount() {return meta.size(); }

    static const Metadata_type& GetLongestKeywordMeta() {
        size_t ms = 0;
        int id = 0;
        for(size_t i = 0; i < meta.size(); i++) {
            if(ms < meta[i].keyword.size()) {
                id = i;
                ms = meta[i].keyword.size();
            }
        }
        return meta[id];
    }

    static T Parse(std::istream& str, Args... args) {
        str >> skipws;
        std::string buf;
        char c;
        size_t maxlen = GetLongestKeywordMeta().keyword.size();
        while(buf.size() < maxlen && str.get(c)) {
            buf += std::tolower(c);
            for(size_t i = 0; i < meta.size(); i++) {
                if(buf == meta[i].keyword) {
                    return meta[i].create_func(str, args...);
                }
            }
        }
        throw parse_error(str, "Unknown keyword: \"" + buf + "\".\nAvailable:\n" + GetFormattedMetadata());
    }

    static std::string GetFormattedMetadata() {
        std::string str = "";
        for(size_t i = 0; i < meta.size(); i++) {
            str += meta[i].ToString() + "\n";
        }
    }
};

template <typename T, typename Metadata_type, typename ... Args>
std::vector<Metadata_type> Parseable<T, Metadata_type, Args...>::meta = std::vector<Metadata_type>();

#endif
