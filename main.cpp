#include "src/argmgr.hpp"
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
    Generator::Init();
    Filter::Init();
    Tune::Init();

#if !defined(_WIN32) && !defined(NDEBUG)
    ::testing::InitGoogleTest(&argc, argv);
#endif

    // Initialize SDL and random
    SDL_Init(SDL_INIT_AUDIO);

    ArgumentManager argmgr{pstate.args};
    argmgr.Parse(argc, argv);
    pstate.ifs = argmgr.getUnparsed();

    //Process and run arguments
    Run();

    // Exit SDL
    SDL_Quit();
#if !defined(_WIN32) && !defined(NDEBUG)
    return RUN_ALL_TESTS();
#else
    return 0;
#endif
}
