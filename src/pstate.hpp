#ifndef L00PR_PROGRAM_STATE
#define L00PR_PROGRAM_STATE

#include "arg.hpp"

// FIXME is a singleton
// Not anymore, I guess??
struct Program {
    // TODO: make hard-coded things into plugins instead
    double seekfwd = 0;
    int stayopen = 1;
    bool opToFile = false;
    bool opCursed = false;
    std::string opFile = "";
    std::vector<std::string> ifs = {};
    std::vector<Argument> args = {};

    Program() {/*nothing*/}
    Program(std::vector<Argument> a) : args(a) {}

    static void setSeek(Program* const, const Argument& arg);
    static void setOpToFile(Program* const, const Argument& arg);
    static void setCursed(Program* const, const Argument&);
    static void printHelp(Program* const, const Argument&);
    static void remainOpen(Program* const, const Argument&);
    static void listSources(Program* const, const Argument&);
    void run();
};

#endif
