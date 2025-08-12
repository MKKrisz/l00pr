#include <filesystem>
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

void Program::setOpToRendered(Program* const p, const Argument& arg) {
    p->output = FILE_RENDERED;
    p->opFile = arg.getArg();
}
void Program::setOpToParsed(Program* const p, const Argument& arg) {
    p->output = FILE_NOT_RENDERED;
    p->opFile = arg.getArg();
}
void Program::disablePlugins(Program* const p, const Argument&) {
    p->plugins = false;
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

void Program::listSources(Program* const, const Argument&) {
    std::cout << "Generators:" << std::endl << Generator::getFormattedMetadata()
        << std::endl << "Filters:" << std::endl << Filter::getFormattedMetadata();
}
void Program::listGenerators(Program* const, const Argument&) {
    std::cout << Generator::getFormattedMetadata();
}
void Program::listFilters(Program* const, const Argument&) {
    std::cout << Filter::getFormattedMetadata();
}

void Program::run() {
    if(ifs.empty()) {
        std::cout << "No input files provided!" << std::endl;
        return;
    }
    
    manager.hook_before_reads(this);

    std::vector<std::filesystem::path> files {};
    files.insert(files.end(), ifs.begin(), ifs.end());

    for(size_t i = 0; i < files.size(); i++) {
        if(!std::filesystem::exists(files[i])) { throw std::runtime_error("File " + files[i].generic_string() + " does not exist!"); }
        if(std::filesystem::is_directory(files[i])) {
            auto dir = files[i];
            files.erase(files.begin() + i);
            i--;
            for(auto file : std::filesystem::directory_iterator(dir)) {
                if(file.path().extension() != ".tn" && !file.is_directory()) {
                    std::cerr << "File " + file.path().generic_string() + " expanded from directory " + dir.generic_string() + " is not a .tn file, skipping!" << std::endl;
                    continue;
                }
                files.emplace_back(file.path());
            }
            continue;
        }
        std::cout << files[i] << std::endl;
        if(files[i].extension() != ".tn") {
            std::cerr << "Directly specified file " + files[i].generic_string() + " is not a .tn file, but will be parsed as one!" << std::endl;
        }
    }
    std::cout << "files resolved" << std::endl;

    Tune tune;
    for(size_t i = 0; i < files.size(); i++) {
        std::fstream file(files[i]);
        try {
            file >> tune;
        }
        catch(std::exception& e) {
            std::cout << "Exception while parsing \"" << files[i] << '"' << std::endl << e.what() << std::endl;
            return;
        }
        file.close();
    }
    tune.resolveReferences();
    manager.hook_after_reads(this);
    switch(output) {
        case AUDIO: device = std::make_unique<AudioDevice>(tune.samplerate()); break;
        case FILE_NOT_RENDERED: device = std::make_unique<PrinterDevice>(tune.samplerate()); break;
        case FILE_RENDERED: device = std::make_unique<RenderDevice>(opFile, tune.samplerate()); break;
        default: break;
    }

    device->addTune(tune);
    device->setCursed(opCursed);
    device->fastForward(seekfwd);

    manager.hook_before_run(this);
    device->start();
    if(!device->isRunning()) {
        manager.hook_after_run(this);
        return;     // Don't wait if device is not realtime
    }
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
