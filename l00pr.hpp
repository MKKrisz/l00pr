#ifndef LIBLIBL00PR
#define LIBLIBL00PR

#include <memory>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "src/device/builtin.hpp"
#include "src/tune/tune.hpp"


class l00pr {
    std::shared_ptr<AudioDevice> dev;
    std::map<std::string, Tune> tune_lib;
public:

    l00pr(int srate, int bufsize) : dev(std::make_shared<AudioDevice>(srate, bufsize)), tune_lib() {}

    bool LoadTune(std::string path) {
        if(tune_lib.contains(path)){
            dev->addTune(tune_lib.find(path)->second);
            return true;
        }
        Tune t;
        std::fstream fs(path);
        try {
            fs >> t;
        }
        catch(std::exception e) {
            std::cout << e.what() << std::endl;
            return false;
        }
        tune_lib.emplace(path, t);
        dev->addTune(t);
        return true;
    }

};

#endif
