#include "plugin_mgr.hpp"

#include <filesystem>

void PluginManager::hook_before_run(Program* const p) {
    for(Plugin& plug : loaded) {
        plug.before_run(p);
    }
}
void PluginManager::hook_after_run(Program* const p) {
    for(Plugin& plug : loaded) {
        plug.after_run(p);
    }
}
void PluginManager::hook_before_reads(Program* const p) {
    for(Plugin& plug : loaded) {
        plug.before_reads(p);
    }
}
void PluginManager::hook_after_reads(Program* const p) {
    for(Plugin& plug : loaded) {
        plug.after_reads(p);
    }
}

std::vector<Argument> PluginManager::arguments() {
    std::vector<Argument> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_args.begin(), plug.custom_args.end());
    }
    return ret;
}

std::vector<Gen_Metadata> PluginManager::generators() {
    std::vector<Gen_Metadata> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_generator_meta.begin(), plug.custom_generator_meta.end());
    }
    return ret;
}
void PluginManager::add_generators() {
    auto metas = generators();
    for(const auto& meta : metas) {
        Generator::AddMetadata(meta);
    }
}

std::vector<Filter_Metadata> PluginManager::filters() {
    std::vector<Filter_Metadata> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_filter_meta.begin(), plug.custom_filter_meta.end());
    }
    return ret;
}

void PluginManager::add_filters() {
    auto metas = filters();
    for(const auto& meta : metas) {
        Filter::AddMetadata(meta);
    }
}

std::vector<Tkwd_Metadata> PluginManager::keywords() {
    std::vector<Tkwd_Metadata> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_tune_meta.begin(), plug.custom_tune_meta.end());
    }
    return ret;
}

void PluginManager::add_keywords() {
    auto metas = keywords();
    for(const auto& meta : metas) {
        Tune::AddMetadata(meta);
    }
}

std::vector<Set_Metadata> PluginManager::values() {
    std::vector<Set_Metadata> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_set_meta.begin(), plug.custom_set_meta.end());
    }
    return ret;
}

void PluginManager::add_values() {
    auto metas = values();
    for(const auto& meta : metas) {
        Set::AddMetadata(meta);
    }
}

std::vector<Note_Metadata> PluginManager::notes() {
    std::vector<Note_Metadata> ret {};
    for(const auto& plug : loaded) {
        ret.insert(ret.end(), plug.custom_note_meta.begin(), plug.custom_note_meta.end());
    }
    return ret;
}

void PluginManager::add_notes() {
    auto metas = notes();
    for(const auto& meta : metas) {
        Note::AddMetadata(meta);
    }
}

void PluginManager::load() {
    if(!std::filesystem::exists(load_dir_path)) {
        std::filesystem::create_directory(load_dir_path);
    }
    if(!std::filesystem::is_directory(load_dir_path)) { throw std::runtime_error("Path '" + load_dir_path + "' is not a directory"); }
    for(const auto& entry : std::filesystem::directory_iterator(load_dir_path)) {
        if(!std::filesystem::is_regular_file(entry)) { continue; }
        bool valid = Plugin::load(entry.path()).has_value();
        if(valid) {
            loaded.emplace_back(Plugin::load(entry.path()).value());
        }
    }
}

PluginManager::~PluginManager() {
    for(auto& plug : loaded) {
        plug.unload();
    }
}
