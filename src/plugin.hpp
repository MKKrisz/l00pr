#ifndef L00PR_PLUGIN
#define L00PR_PLUGIN

#include "arg.hpp"
#include "audiosource.hpp"
#include "tune/tune.hpp"
#include "tune/set_kwd.hpp"
#include "device/builtin.hpp"
#include "generator/generator.hpp"
#include "filter/filter.hpp"
#include <functional>
#include <concepts>

//template<invokable F>
//class FunctionProvider {
//    
//};
extern "C" { 

class Program;

struct Plugin {
    std::string name = "";

    std::vector<Argument> custom_args {};

    std::vector<Filter_Metadata> custom_filter_meta {};
    std::vector<Gen_Metadata> custom_generator_meta {};
    std::vector<Tkwd_Metadata> custom_tune_meta {};
    std::vector<Set_Metadata> custom_set_meta {};
    std::vector<Note_Metadata> custom_note_meta {};
    
    void (*hook_before_run)(Program* const) = nullptr;
    void before_run(Program* const p) {
        if(hook_before_run == nullptr) {return;}
        hook_before_run(p);
    }

    void (*hook_after_run)(Program* const) = nullptr;
    void after_run(Program* const p) {
        if(hook_after_run == nullptr) {return;}
        hook_after_run(p);
    }

    void (*hook_before_reads)(Program* const) = nullptr;
    void before_reads(Program* const p) {
        if(hook_before_reads == nullptr) {return;}
        hook_before_reads(p);
    }

    void (*hook_after_reads)(Program* const) = nullptr;
    void after_reads(Program* const p) {
        if(hook_after_reads == nullptr) {return;}
        hook_after_reads(p);
    }
    
    static std::optional<Plugin> load(const std::string& path);

    void unload();

private:
    void* handle = 0;
};
}

#endif
