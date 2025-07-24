#ifndef L00PR_PARSEABLE
#define L00PR_PARSEABLE

#include <string>
#include <vector>
#include <functional>
#include <cstdarg>
#include <istream>
#include <optional>

#include "exceptions/parse_error.hpp"
#include "exceptions/no_keyword.hpp"
#include "util.hpp"
#include "string_convertible.hpp"
#include "writeable.hpp"

template <typename T, typename ... Args>
class Metadata : StringConvertible {
protected:
    Metadata(const char* kw, std::function<T(std::istream&, Args...)> fn) : keyword(kw), create_func(fn) {}
    Metadata(const Metadata& m) : keyword(m.keyword), create_func(m.create_func) {}
public:
    std::string keyword;
    std::function<T(std::istream&, Args...)> create_func;
    virtual std::string ToString() const { return keyword; }
};

///<summary> Base class for all classes that 
template <typename T, typename Metadata_type, typename ... Args>
class Parseable : public virtual Writeable {
protected:
    static std::vector<Metadata_type> meta;
    static std::optional<Metadata_type> default_meta;
public:
    static void AddMetadata(const Metadata_type& m) { meta.emplace_back(m); }
    static void ClearMetadata() {meta.clear();}

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
        auto idx = str.tellg();
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
        if(default_meta.has_value()) {
            try {
                str.seekg(idx);
                return default_meta.value().create_func(str, args...);
            }
            catch (no_such_keyword&) {/*nothing, signals to the class that the data was unrecogniseable*/}
        }
        throw no_such_keyword(str, "Unknown keyword: \"" + buf + "\".\nAvailable:\n" + GetFormattedMetadata());
    }

    static std::string GetFormattedMetadata() {
        std::string str = "";
        for(size_t i = 0; i < meta.size(); i++) {
            str += meta[i].ToString() + "\n";
        }
        return str;
    }
};

template <typename T, typename Metadata_type, typename ... Args>
std::vector<Metadata_type> Parseable<T, Metadata_type, Args...>::meta = std::vector<Metadata_type>();

template <typename T, typename Metadata_type, typename ... Args>
std::optional<Metadata_type> Parseable<T, Metadata_type, Args...>::default_meta = std::nullopt;

#endif
