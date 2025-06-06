#pragma once

#include "plugin.hpp"
#include "generator/generator.hpp"

class PluginManager {
public:
    PluginManager(std::string path) : load_dir_path(path) {}
    ~PluginManager();

    void load();

    void hook_before_run(Program* const);
    void hook_after_run(Program* const);
    void hook_before_reads(Program* const);
    void hook_after_reads(Program* const);

    std::vector<Argument> arguments();
    std::vector<Gen_Metadata> generators();
    void add_generators();
    std::vector<Filter_Metadata> filters();
    void add_filters();
    std::vector<Tkwd_Metadata> keywords();
    void add_keywords();
    std::vector<Set_Metadata> values();
    void add_values();
    std::vector<Note_Metadata> notes();
    void add_notes();

    void add_extensions() {
        add_generators();
        add_filters();
        add_keywords();
        add_values();
        add_notes();
    }

    static PluginManager load(std::string path) {
        PluginManager mgr(path);
        mgr.load();
        return mgr;
    }
private:
    std::string load_dir_path;
    std::vector<Plugin> loaded;
};
