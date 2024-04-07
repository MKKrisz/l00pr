#ifndef L00PR_GENERATORS_NONE
#define L00PR_GENERATORS_NONE

#include "../generator.hpp"

class NoGenerator : public Generator {
public: 
    NoGenerator();
    float getSample(float phase, float t);
    Generator* copy();
};

#endif
