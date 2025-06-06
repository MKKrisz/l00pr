#pragma once

#include <ostream>

class Writeable {
public:
    virtual void Write(std::ostream&) const = 0;
};

