#ifndef L00PR_PLUGIN
#define L00PR_PLUGIN

#include "arg.hpp"
#include "audiosource.hpp"
#include "filter/filter.hpp"
#include "generator/generator.hpp"
#include <optional>
#include <functional>

class Plugin {
    std::vector<Argument> custom_args;

    std::vector<std::string> custom_filter_names;
    std::vector<std::string> custom_generator_names;
    std::pair<std::string, std::function<void(std::istream&, int)>> custom_file_keywords;
    std::pair<std::string, std::function<void(std::istream&, int)>> custom_set_values;


    std::optional<std::function<AudioSource*(std::istream&, const int, const MakeFlags&)>> custom_as_make;
    std::optional<std::function<Filter*(std::string&, std::istream&, const int, const MakeFlags&)>> custom_filter_make;
    std::optional<std::function<Generator*(std::string&, std::istream&, const int, const MakeFlags&)>> custom_generator_make;

};

#endif
