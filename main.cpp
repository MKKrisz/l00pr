#include "src/argmgr.hpp"
#include <SDL2/SDL.h>
#include <ctime>

int main(int argc, const char** argv) {
    SDL_Init(SDL_INIT_AUDIO);
    srand(time(0));
    Arguments args(argc, argv);
    args.Run();
    SDL_Quit();
}
