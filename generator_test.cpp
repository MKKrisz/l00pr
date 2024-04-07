
#define GENERATOR_TEST
#ifdef GENERATOR_TEST

#include "generators/builtin.hpp"
#include <iostream>

int main() {
    float phase = 0;
    TriangleGenerator g{1, 0, 0};

    while(true) {
        float v = g.getSample(phase) * 10;
        for(int i = -10; i< v; i++){
            std::cout << ' ';
        }
        std::cout << '#' << std::endl;

        phase += 0.03f;    
    }
}

#endif
