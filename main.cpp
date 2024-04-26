#include "src/argmgr.hpp"
#include <SDL2/SDL.h>
#include <ctime>

/// <summary>
/// Entry point of the code. Standard parameters, nothing special
/// </summary>
int main(int argc, const char** argv) {
    // Initialize SDL and random
    SDL_Init(SDL_INIT_AUDIO);
    srand(time(0));

    //Process and run arguments
    Arguments args(argc, argv);
    args.Run();

    // Exit SDL
    SDL_Quit();
}
