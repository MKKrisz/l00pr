#ifndef L00PR_PROGRAM_STATE
#define L00PR_PROGRAM_STATE

#include "arg.hpp"
#include "plugin_mgr.hpp"
#include "device/device.hpp"

#include <memory>

enum OutputType {
    AUDIO, FILE_RENDERED, FILE_NOT_RENDERED
};

// FIXME is a singleton
// Not anymore, I guess??
struct Program {
    bool plugins = true;
    // TODO: make hard-coded things into plugins instead
    double seekfwd = 0;
    int stayopen = 1;
    OutputType output = AUDIO;
    bool opCursed = false;
    std::string opFile = "";
    std::vector<std::string> ifs = {};
    std::vector<Argument> args = {};

    std::unique_ptr<Device> device = nullptr;
    PluginManager manager {"plugins/"};

    Program() {/*nothing*/}
    Program(std::vector<Argument> a) : args(a) {}

    static void setSeek(Program* const, const Argument& arg);
    static void setOpToRendered(Program* const, const Argument& arg);
    static void setOpToParsed(Program* const, const Argument&);
    static void setCursed(Program* const, const Argument&);
    static void printHelp(Program* const, const Argument&);
    static void remainOpen(Program* const, const Argument&);
    static void listGenerators(Program* const, const Argument&);
    static void listFilters(Program* const, const Argument&);
    static void listSources(Program* const, const Argument&);
    static void disablePlugins(Program* const, const Argument&);
    void run();
};

#endif
