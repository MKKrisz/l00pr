#include "argmgr.hpp"
#include <stdexcept>


// TODO: Refactor this mess
void ArgumentManager::parse(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        std::string c_arg = argv[i];

        // If does not start with a '-', consider it as not an argument
        if(c_arg[0] != '-') {
            unparseable.emplace_back(c_arg);
            continue;
        }

        // If argument is a single char arg
        if(c_arg[1] != '-') {
            // Search for arg char
            for(size_t j = 1; j < c_arg.size(); j++) {
                char current = c_arg[j];
                size_t arg_id = 0;
                for(; arg_id < args.size(); arg_id++) {
                    if(args[arg_id].match(current))
                        break;
                }
                if(arg_id >= args.size())
                    throw std::runtime_error(std::string("No char argument '") + current + "' exists");
                
                Argument& f_arg = args[arg_id];

                // If found
                // If it forbids an argument, 
                if(f_arg.getArgOpt() == FORBIDS_ARG) {
                    // Set it and move on to the next char
                    f_arg.set();
                    continue;
                }
                
                // If it is the last char in the string
                if(j == c_arg.size()-1) {
                    bool nextExists = i < argc - 1;
                    
                    // If it requires an argument
                    if(f_arg.getArgOpt() == REQUIRES_ARG) {
                        // And there is none, throw an error!
                        if(!nextExists)
                            throw std::runtime_error("Not enough arguments!");

                        // Otherwise, set the next element of argv as an argument, no matter what it is
                        f_arg.setArg(argv[++i]);
                    }

                    // If it can have an argument, and one exists and it is in fact, not another argument, set it
                    else if(nextExists && argv[i+1][0] != '-') 
                        f_arg.setArg(argv[++i]);
                }

                // If it is not, then set the remainder of the string to be the argument
                else f_arg.setArg(&(argv[i][j+1]));
                break;
            }
            continue;
        }
        for(size_t j = 0; j < args.size(); j++) {
            if(!args[j].match(&(argv[i][2])))
                continue;

            if(args[j].getArgOpt() == FORBIDS_ARG) {
                args[j].set();
                continue;
            }

            if(i + 1 >= argc)
                throw std::runtime_error("Not enough arguments!");
            args[j].setArg(argv[++i]);
        }
    }
}


void ArgumentManager::setup(Program* const p) {
    for(const Argument& arg : args) {
        arg.setup(p);
    }
}
