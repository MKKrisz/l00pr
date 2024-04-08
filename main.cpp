
#include <thread>

#include <fstream>
#include <iostream>
#include "src/tune/tune.hpp"
#include "src/device.h"
#include "src/exceptions/parse_error.hpp"

int main(int argc, const char** argv) {
    SDL_Init(SDL_INIT_AUDIO);
    std::fstream f = std::fstream(argc>=2?argv[1]:"test.tn");
    Tune t;
    try{
        f >> t;
    }
    catch(parse_error e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    AudioDevice device = AudioDevice(t.getSampleRate());
    device.setTune(&t);
    device.start();
    sleep(t.getLen() + 1);
    device.stop();
    SDL_Quit();
}
