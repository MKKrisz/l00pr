#pragma once

#include "parse_error.hpp"

class no_such_keyword : public parse_error {
public:
    no_such_keyword() : parse_error() {}
    no_such_keyword(std::istream& stream, const std::string& what) : parse_error(stream, what) {}
    no_such_keyword(std::istream& stream, const char* what) : parse_error(stream, what) {}
    no_such_keyword(int line, int col, const std::string& what) : parse_error(line, col, what) {}
    no_such_keyword(int line, int col, const char* what) : parse_error(line, col, what) {}
};
