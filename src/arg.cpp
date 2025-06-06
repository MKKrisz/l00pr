#include "arg.hpp"
#include "pstate.hpp"

std::vector<Argument> Argument::getDefault() {
    return {
        {'h', "help", "Prints this message", Program::printHelp, FORBIDS_ARG},
        {'s', "seek-forward", "Seeks forward 't' seconds", Program::setSeek, REQUIRES_ARG},
        {'o', "output", "Instead of playing, the program will export the tune to 'f' in a wav format", Program::setOpToRendered, REQUIRES_ARG},
        {'p', "parse-back", "Instead of playing, the program will just print the parsed file back to the terminal in hopefully a form that can be parsed back", Program::setOpToParsed, FORBIDS_ARG},
        {'c', "cursed", "Reverts to using the first working version of the wav exporter", Program::setCursed, FORBIDS_ARG},
        {'r', "remain-open", "Keeps the program open for 'n' seconds after the tune is played", Program::remainOpen, REQUIRES_ARG},
        {"list-sources", "Lists all available generators and filters, their syntax, and a simple explanation about what they do.", Program::listSources, FORBIDS_ARG},
        {"list-generators", "Lists all available generators, their syntax, and a simple explanation about what they do.", Program::listGenerators, FORBIDS_ARG},
        {"list-filters", "Lists all available filters, their syntax, and a simple explanation about what they do.", Program::listFilters, FORBIDS_ARG},
        {"disable-plugins", "Disables loading of plugins.", Program::disablePlugins, FORBIDS_ARG},
    };
}
