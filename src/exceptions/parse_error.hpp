#ifndef L00PR_PARSE_ERROR
#define L00PR_PARSE_ERROR

#include <exception>
#include <string>

/// <summary> Exception to throw when parsing error occurs. Also calculates stream position. </summary>
class parse_error : public std::exception {
    /// <summary> The error string </summary>
    /// <remarks> No thats not a typo </remarks>
    std::string ehat;
public:

    // Various constructors
    parse_error();
    parse_error(std::istream& stream, const std::string& what);
    parse_error(std::istream& stream, const char* what);
    parse_error(int line, int col, const std::string& what);
    parse_error(int line, int col, const char* what);

    /// <summary> Returns the error message </summary>
    const char* what() const noexcept;

    // Copy assignment operator
    parse_error& operator=(const parse_error&);
};

#endif
