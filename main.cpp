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

    Program program{Argument::getDefault()};

    //Plugin code goes here

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
