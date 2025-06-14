#ifndef L00PR_GENERATORS_NONE
#define L00PR_GENERATORS_NONE

#include "generator.hpp"

//TODO: Consider making a pointer to a copy of this the default argument for most AudioSource* values instead of nullptr
/// <summary> 
/// Generates a constant 0 sample. Serves no purpose, you can't even make one.
/// </summary>
class NoGenerator : public Generator {
public: 
    NoGenerator();
    double getSample(double, double);
    std::unique_ptr<AudioSource> copy();
    std::string ToString() const { return "Dummy"; }
    void Write(std::ostream& str) const {str << "none";}

    static std::unique_ptr<NoGenerator> Create(std::istream& str, const int, const MakeFlags&);
};

#endif
