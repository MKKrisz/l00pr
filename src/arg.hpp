#ifndef L00PR_ARG
#define L00PR_ARG

#include <string>
#include <functional>

enum ArgumentOption{
    FORBIDS_ARG, ALLOWS_ARG, REQUIRES_ARG
};

class Argument {
    char c;
    std::string str;
    std::string helpstr;
    ArgumentOption argOpt;

    bool is_set = false;
    std::string arg = "";
    std::function<void(const Argument&)> fn;
public:
    Argument(char c, const std::string& str, const std::string& help, std::function<void(const Argument&)> fn, ArgumentOption argOpt = ALLOWS_ARG)
        : c(c), str(str), helpstr(help), argOpt(argOpt), fn(fn) {}

    Argument(const std::string& str, const std::string& help, std::function<void(const Argument&)> fn, ArgumentOption argOpt = ALLOWS_ARG) 
        : c(0), str(str), helpstr(help), argOpt(argOpt), fn(fn) {}

    char getChar() const { return c; }
    const std::string& getStr() const { return str; }
    const std::string& getHelp() const { return helpstr; }
    ArgumentOption getArgOpt() const { return argOpt; }

    bool match(const char* c) { return str == c; }
    bool match(const char c) { return this->c == c; }

    void set() { is_set = true; }
    bool get_is_set() const { return is_set; }
    void setArg(const std::string& str) {
        is_set = true;
        arg = str;
    }
    const std::string& getArg() const { return arg; }
    void setup() { if(is_set) fn(*this); }
};

#endif
