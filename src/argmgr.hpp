#ifndef L00PR_ARGMGR
#define L00PR_ARGMGR

#include <string>
#include <vector>

class Arguments {
    double seekfwd = 0;
    std::vector<std::string> inputFiles;
    bool outputToFile = false;
    std::string outputFile;

public:
    Arguments(int argc, const char** argv);
    void Run();
};

#endif
