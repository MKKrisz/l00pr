#ifndef L00PR_PARSE_ERROR
#define L00PR_PARSE_ERROR

#include <exception>
#include <string>

class parse_error : public std::exception {
    std::string ehat;
public:
    parse_error();
    parse_error(std::istream& stream, const std::string& what);
    parse_error(std::istream& stream, const char* what);
    parse_error(int line, int col, const std::string& what);
    parse_error(int line, int col, const char* what);

    const char* what() const noexcept;

    parse_error& operator=(const parse_error&);
};

#endif
