#include "noise.hpp"

NoiseGenerator::NoiseGenerator(std::istream& stream) : Generator(stream){
    if(shouldBeDefault) { 
        parse_lb(stream);
        return;
    }
    if((stream >> skipws).peek() != ')')  
        throw parse_error(stream, "Excepted ')'");
    stream.get();
    parse_lb(stream);
}
