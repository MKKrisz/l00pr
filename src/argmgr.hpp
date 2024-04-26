#ifndef L00PR_ARGMGR
#define L00PR_ARGMGR

#include <string>
#include <vector>

/// <summary>
/// Argument manager. Processes arguments, then executes the program accordingly.
/// </summary>
class Arguments {
    /// <summary> The value to seek forward at the beginning of playing measured in seconds. </summary>
    double seekfwd = 0;

    /// <summary> The file names that need to be parsed. </summary>
    std::vector<std::string> inputFiles;

    /// <summary> Flag set when program needs to output to file. </summary>
    bool outputToFile = false;

    /// <summary> Flag set when the "-c" or "--cursed" flag is specified. </summary>
    bool cursed = false;

    /// <summary> Name of the file to output into. The file will be overwritten. </summary>
    std::string outputFile;

public:
    /// <summary> Constructor that processes standard arguments </summary>
    Arguments(int argc, const char** argv);

    /// <summary> Parses files, than runs the program according to member variables </summary>
    void Run();
};

#endif
