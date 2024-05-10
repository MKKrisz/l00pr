#include "argmgr.hpp"

void ArgumentManager::Parse(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        std::string c_arg = argv[i];
        if(c_arg[0] != '-') {
            unparseable.emplace_back(c_arg);
            continue;
        }
        if(c_arg[1] != '-') {
            bool parsed = false;
            for(size_t j = 1; !parsed && j < c_arg.size(); j++) {
                char current = c_arg[j];
                for(size_t k = 0; k < args.size(); k++) {
                    if(!args[k].match(current))
                        continue;

                    if(args[k].getArgOpt() == FORBIDS_ARG) {
                        args[k].set();
                    }
                    else {
                        if(c_arg.size() == j+1) {
                            if(args[k].getArgOpt() == REQUIRES_ARG) 
                                args[k].setArg(argv[++i]);
                            else if(argv[i+1][0] != '-') 
                                args[k].setArg(argv[++i]);
                        }
                        else args[k].setArg(&(argv[i][j+1]));
                        parsed = true;
                    }
                    break;
                }
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
            args[j].setArg(argv[++i]);
        }
    }
    for(size_t i = 0; i < args.size(); i++){
        args[i].setup();
    }
}
