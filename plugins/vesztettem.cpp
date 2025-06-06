#include "vesztettem.hpp"
#include <iostream>

void vesztettem(Program* const) { std::cout << "Vesztettem" << std::endl; }

extern "C" { void Init(Plugin* p) {
    p->name = "Vesztettem";
    p->hook_after_run = vesztettem;
    p->hook_before_run = vesztettem;
    p->hook_before_reads = vesztettem;
    p->hook_after_reads = vesztettem;
}
}
