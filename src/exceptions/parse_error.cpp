#include "parse_error.hpp"
#include "../util.hpp"

#include <sstream>

parse_error::parse_error() : ehat("A parsing error occured") {}
parse_error::parse_error(std::istream& stream, const std::string& what) 
    : parse_error(getLineColNo(stream).first, getLineColNo(stream).second, what) {}

parse_error::parse_error(std::istream& stream, const char* what) 
    : parse_error(getLineColNo(stream).first, getLineColNo(stream).second, what) {}

parse_error::parse_error(int line, int col, const char* what) 
    : parse_error(line, col, std::string(what)) {}

parse_error::parse_error(int line, int col, const std::string& what) {
    std::stringstream buf;
    buf << "Parsing error at " << line << ',' << col << ": " << what;
    ehat = buf.str();
}


const char* parse_error::what() const noexcept {
    return ehat.c_str();
}


parse_error& parse_error::operator=(const parse_error& e) {
    if(this != &e)
        ehat = e.ehat;
    return *this;
}
