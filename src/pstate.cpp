#include <iostream>
#include <fstream>
#include <thread>

#include "pstate.hpp"
#include "tune/tune.hpp"
#include "device.h"

program_state pstate = {
    0,
    false,
    false,
    "",
    {},
    {
        {'h', "help", "Prints this message", printHelp, FORBIDS_ARG},
        {'s', "seek-forward", "Seeks forward 't' seconds", setSeek, REQUIRES_ARG},
        {'o', "output", "Instead of playing, the program will export the tune to 'f' in a wav format", setOpToFile, REQUIRES_ARG},
        {'c', "cursed", "Reverts to using the first working version of the wav exporter", setCursed, FORBIDS_ARG},
    }
};


void setOpToFile(const Argument& arg) {
    pstate.opToFile = true;
    pstate.opFile = arg.getArg();
}

void setCursed(const Argument&) { pstate.opCursed = true; }
void setSeek(const Argument& arg) { pstate.seekfwd = stof(arg.getArg()); }


void printHelp(const Argument&) {
    std::cout << "USAGE: l00pr [arguments] <files>" << std::endl;
    for(auto arg : pstate.args) {
        std::cout << '-' << arg.getChar() << "    " << "--" << arg.getStr() << "\t" << (arg.getStr().size() > 8 ?"" : "\t") << arg.getHelp() << std::endl;
    }
}

void Run() {
    if(pstate.ifs.empty()) {
        std::cout << "No input files provided!" << std::endl;
        return;
    }
    Tune tune;
    for(size_t i = 0; i < pstate.ifs.size(); i++) {
        std::fstream file(pstate.ifs[i]);
        try {
            file >> tune;
        }
        catch(std::exception& e) {
            std::cout << "Exception while parsing \"" << pstate.ifs[i] << '"' << std::endl << e.what() << std::endl;
            return;
        }
        file.close();
    }
    AudioDevice dev{tune.getSampleRate()};
    dev.addTune(tune);

    if(!pstate.opToFile) {
        dev.fastForward(pstate.seekfwd);
        dev.start();
        uint len = tune.getLen();
        if(len == std::numeric_limits<double>::infinity()) 
            len = std::numeric_limits<uint>::max();
        else 
            len++;
        sleep(len - pstate.seekfwd);
        dev.stop();
        return;
    }
    std::ofstream file(pstate.opFile);
    dev.render(file, pstate.opCursed);
    file.close();
}
