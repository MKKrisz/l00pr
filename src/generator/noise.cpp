#include "noise.hpp"

NoiseGenerator::NoiseGenerator(std::istream& stream) : Generator(stream){
    if(shouldBeDefault) return;
    if((stream >> skipws).peek() != ')')  
        throw parse_error(stream, "Excepted ')'");
    stream.get();
}
