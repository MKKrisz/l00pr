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
#include "device/builtin.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"

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
    std::cout << "Generators:" << std::endl << Generator::getFormattedMetadata()
        << std::endl << "Filters:" << std::endl << Filter::getFormattedMetadata();
}
void Program::listGenerators(Program* const p, const Argument&) {
    std::cout << Generator::getFormattedMetadata();
}
void Program::listFilters(Program* const p, const Argument&) {
    std::cout << Filter::getFormattedMetadata();
}

void Program::run() {
    if(ifs.empty()) {
        std::cout << "No input files provided!" << std::endl;
        return;
    }
    
    manager.hook_before_reads(this);

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
    manager.hook_after_reads(this);

    if(opToFile) { 
        device = std::make_unique<RenderDevice>(opFile, tune.samplerate()); 
    } 
    else { 
        device = std::make_unique<AudioDevice>(tune.samplerate()); 
    }

    device->addTune(tune);
    device->setCursed(opCursed);
    device->fastForward(seekfwd);

    manager.hook_before_run(this);
    device->start();
    if(!device->isRunning()) {return;}     // Don't wait if device is not realtime
    double len = tune.getLen() - seekfwd + stayopen + 1;
    uint u_len = len;
    if(len == std::numeric_limits<double>::infinity()) {
#ifdef _WIN32
        u_len = UINT_MAX;
#else
        u_len = std::numeric_limits<uint>::max();
#endif
    }

    sleep(u_len);
    device->stop();
    manager.hook_after_run(this);
}
