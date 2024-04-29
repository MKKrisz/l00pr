#include "src/argmgr.hpp"
#include "src/pstate.hpp"
#include <SDL2/SDL.h>
#include <ctime>


/// <summary>
/// Entry point of the code. Standard parameters, nothing special
/// </summary>
int main(int argc, const char** argv) {
    // Initialize SDL and random
    SDL_Init(SDL_INIT_AUDIO);
    srand(time(0));

    ArgumentManager argmgr{args};
    argmgr.Parse(argc, argv);
    ifs = argmgr.getUnparsed();

    //Process and run arguments
    Run();

    // Exit SDL
    SDL_Quit();
}
