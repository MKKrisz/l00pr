#include "src/argmgr.hpp"
#include "src/pstate.hpp"
#include "src/generator/generator.hpp"
#include "src/filter/filter.hpp"
#include "src/tune/tune.hpp"
#include <SDL2/SDL.h>
#ifndef NDEBUG
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

#ifndef NDEBUG
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
#ifndef NDEBUG
    return RUN_ALL_TESTS();
#endif
}
