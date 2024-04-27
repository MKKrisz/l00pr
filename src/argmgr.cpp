#include <iostream>
#include <fstream>
#include <thread>
#include "argmgr.hpp"
#include "tune/tune.hpp"
#include "device.h"

Arguments::Arguments(int argc, const char** argv) : inputFiles() {
    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-h" || arg == "--help") {
            std::cout << "Usage: l00pr [arguments] <tune file>" << std::endl
                << std::endl << "Arguments:" << std::endl 
                << "-h\t--help\t\t\tPrints this message" << std::endl
                << "-s <t>\t--seek-forward <t>\tSeeks forward 't' seconds" << std::endl
                << "-o <f>\t--output <f>\t\tInstead of playing, the program will export the tune to 'f' in a wav format" << std::endl
                << "-c\t--cursed\t\t\tReverts to using the first working version of the wav exporter. << std::endl";
            continue;
        }
        if(arg == "-s" || arg == "--seek-forward") {
            i++;
            seekfwd = atof(argv[i]);
            continue;
        }
        if(arg == "-o" || arg == "--output") {
            outputToFile = true;
            i++;
            outputFile = argv[i];
            continue;
        }
        if(arg == "-c" || arg == "--cursed") {
            cursed = true;
            continue;
        }
        inputFiles.emplace_back(argv[i]);
    }
}
void Arguments::Run() {
    if(inputFiles.empty()) {
        std::cout << "No input files provided!" << std::endl;
        return;
    }
    Tune tune;
    for(size_t i = 0; i < inputFiles.size(); i++) {
        std::fstream file(inputFiles[i]);
        try {
            file >> tune;
        }
        catch(std::exception& e) {
            std::cout << "Exception while parsing \"" << inputFiles[i] << '"' << std::endl << e.what() << std::endl;
            return;
        }
        file.close();
    }
    AudioDevice dev{tune.getSampleRate()};
    dev.addTune(tune);
    if(!outputToFile) {
        dev.fastForward(seekfwd);
        dev.start();
        uint len = tune.getLen();
        if(len == std::numeric_limits<double>::infinity()) 
            len = std::numeric_limits<uint>::max();
        else 
            len++;
        sleep(len - seekfwd + 1);
        dev.stop();
        return;
    }
    std::ofstream file(outputFile);
    if(cursed)
        dev.renderCursed(file);
    else
        dev.render(file);
    file.close();
}
