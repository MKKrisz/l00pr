#ifndef L00PR_ARGMGR
#define L00PR_ARGMGR

#include <string>
#include <vector>
#include "arg.hpp"

class ArgumentManager {
    std::vector<Argument> args;
    std::vector<std::string> unparseable;

public: 
    ArgumentManager(std::vector<Argument> args) : args(args), unparseable() {}
    void Parse(int argc, char** argv);
    std::vector<std::string> getUnparsed() { return unparseable; }
    size_t getArgCount() { return args.size(); }
    Argument& operator[](size_t i) {
        return args[i];
    }
};

#endif
