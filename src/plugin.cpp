#include "plugin.hpp"

#include <dlfcn.h>

std::optional<Plugin> Plugin::load(const std::string& path) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    std::cerr << dlerror() << std::endl;
    if(handle == nullptr) { return std::nullopt; }
    std::function<void(Plugin*)> init_fn = (void (*)(Plugin*))dlsym(handle, "Init");
    std::cerr << dlerror() << std::endl;

    if(init_fn == nullptr) {throw std::runtime_error("Plugin does not implement required function 'Init'"); }
    Plugin plug;
    init_fn(&plug);
    plug.handle = handle;
    return plug;
}

void Plugin::unload() {
    if(handle != nullptr) {
        dlclose(handle);
    }
}
