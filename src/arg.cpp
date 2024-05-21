#include "arg.hpp"
#include "pstate.hpp"

std::vector<Argument> Argument::getDefault() {
    return {
        {'h', "help", "Prints this message", Program::printHelp, FORBIDS_ARG},
        {'s', "seek-forward", "Seeks forward 't' seconds", Program::setSeek, REQUIRES_ARG},
        {'o', "output", "Instead of playing, the program will export the tune to 'f' in a wav format", Program::setOpToFile, REQUIRES_ARG},
        {'c', "cursed", "Reverts to using the first working version of the wav exporter", Program::setCursed, FORBIDS_ARG},
        {'r', "remain-open", "Keeps the program open for 'n' seconds after the tune is played", Program::remainOpen, REQUIRES_ARG}
    };
}
