#ifndef L00PR_STRING_CONVERTIBLE
#define L00PR_STRING_CONVERTIBLE

#include <string>

class StringConvertible {
public:
    virtual std::string ToString() const = 0;
};

#endif
