#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <cstdint>
#define sleep(x) Sleep(1000 * (x))
typedef unsigned int uint;
#else
#include <unistd.h>
#endif

#include "pstate.hpp"
#include "tune/tune.hpp"
#include "device.h"
#include "audiosource.hpp"

void Program::setOpToFile(Program* const p, const Argument& arg) {
    p->opToFile = true;
    p->opFile = arg.getArg();
}

void Program::setCursed(Program* const p, const Argument&) { 
    p->opCursed = true; 
}

void Program::setSeek(Program* const p, const Argument& arg) { 
    p->seekfwd = stof(arg.getArg()); 
}

void Program::printHelp(Program* const p, const Argument&) {
    std::cout << "USAGE: l00pr [arguments] <files>" << std::endl;
    for(auto arg : p->args) {
        std::cout << (arg.getChar() == 0 ? "" : "-") << arg.getChar() << (arg.getChar() == 0 ? "      ":"    ") << "--" << arg.getStr() << "\t" << (arg.getStr().size() > 8 ?"" : "\t") << arg.getHelp() << std::endl;
    }
}

void Program::remainOpen(Program* const p, const Argument& arg) {
    p->stayopen = stoi(arg.getArg());
}

void Program::listSources(Program* const p, const Argument&) {
    std::cout << AudioSource::getFormattedMetadata();
}

void Program::run() {
    if(ifs.empty()) {
        std::cout << "No input files provided!" << std::endl;
        return;
    }
    Tune tune;
    for(size_t i = 0; i < ifs.size(); i++) {
        std::fstream file(ifs[i]);
        try {
            file >> tune;
        }
        catch(std::exception& e) {
            std::cout << "Exception while parsing \"" << ifs[i] << '"' << std::endl << e.what() << std::endl;
            return;
        }
        file.close();
    }
    AudioDevice dev{tune.getSampleRate()};
    dev.addTune(tune);
    dev.fastForward(seekfwd);

    if(!opToFile) {
        dev.start(opCursed);
        uint len = tune.getLen();
        if(len == std::numeric_limits<double>::infinity())
#ifdef _WIN32
            len = UINT_MAX;
#else
            len = std::numeric_limits<uint>::max();
#endif
        else 
            len++;

        //TODO: use a loop that keeps the program awake
        sleep(len - seekfwd + stayopen);
        dev.stop();
    }
    else {
        std::ofstream file(opFile);
        if(!file.good())
            throw std::runtime_error("Could not open file: " + opFile + " for writing!");
        dev.render(file, opCursed);
        file.close();
    }
}
