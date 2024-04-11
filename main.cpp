
#include <thread>

#include <fstream>
#include <iostream>
#include "src/tune/tune.hpp"
#include "src/device.h"
#include "src/exceptions/parse_error.hpp"

int main(int argc, const char** argv) {
    SDL_Init(SDL_INIT_AUDIO);
    srand(time(0));
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
    uint len = t.getLen() == std::numeric_limits<double>::infinity()? std::numeric_limits<uint>::max() : t.getLen() + 1;
    sleep(len);
    device.stop();
    SDL_Quit();
}
