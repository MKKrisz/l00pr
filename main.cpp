#include "src/argmgr.hpp"
#include "src/arg.hpp"
#include "src/pstate.hpp"
#include "src/generator/generator.hpp"
#include "src/filter/filter.hpp"
#include "src/tune/tune.hpp"

#ifdef _WIN32
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#if !defined(_WIN32) && !defined(NDEBUG)
#include <gtest/gtest.h>
#endif

#include <ctime>


/// <summary>
/// Entry point of the code. Standard parameters, nothing special
/// </summary>
int main(int argc, char** argv) {
    srand(time(0));
    SDL_Init(SDL_INIT_AUDIO);

    Generator::Init();
    Filter::Init();
    Tune::Init();
    Note::Init();

    ArgumentManager unplugger{
        {Argument("disable-plugins", "Disables loading of plugins.", Program::disablePlugins, FORBIDS_ARG)}
    };
    try {
        unplugger.parse(argc, argv);
    }
    catch(std::exception& e) {
        //std::cout << e.what() << std::endl;
    }

    Program program{Argument::getDefault()};
    unplugger.setup(&program);

    if(program.plugins) {
        program.manager.load();
        program.manager.add_extensions();
        auto plugin_args = program.manager.arguments();
        program.args.insert(program.args.end(), plugin_args.begin(), plugin_args.end());
    }

    ArgumentManager argmgr{program.args};
    try {
        argmgr.parse(argc, argv);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        goto exit;
    }
    argmgr.setup(&program);
    program.ifs = argmgr.getUnparsed();

    try {
        program.run();
    } 
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

exit:

    // Exit SDL
    SDL_Quit();
#if !defined(_WIN32) && !defined(NDEBUG)
    //return RUN_ALL_TESTS();
#else
    return 0;
#endif
}
