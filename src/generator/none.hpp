#ifndef L00PR_GENERATORS_NONE
#define L00PR_GENERATORS_NONE

#include "generator.hpp"

class NoGenerator : public Generator {
public: 
    NoGenerator();
    double getSample(double phase, double t);
    Generator* copy();
};

#endif
