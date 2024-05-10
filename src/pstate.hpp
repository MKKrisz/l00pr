#ifndef L00PR_PROGRAM_STATE
#define L00PR_PROGRAM_STATE

#include "arg.hpp"

struct program_state {
    double seekfwd;
    bool opToFile;
    bool opCursed;
    std::string opFile;
    std::vector<std::string> ifs;
    std::vector<Argument> args; 

};

extern program_state pstate;

void setSeek(const Argument& arg);
void setOpToFile(const Argument& arg);
void setCursed(const Argument&);
void printHelp(const Argument&);
void Run();

#endif
