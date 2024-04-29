#ifndef L00PR_PROGRAM_STATE
#define L00PR_PROGRAM_STATE

#include "arg.hpp"

// Program states
extern double seekfwd;
extern bool opToFile;
extern bool opCursed;
extern std::string opFile;
extern std::vector<std::string> ifs;
extern std::vector<Argument> args;

void setOpToFile(const Argument& arg);
void setCursed(const Argument&);
void setSeek(const Argument& arg);
void printHelp(const Argument&);
void Run();

#endif
