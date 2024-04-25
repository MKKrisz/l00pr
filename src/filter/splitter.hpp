#ifndef L00PR_FILTER_SPLITTER
#define L00PR_FILTER_SPLITTER

#include "filter.hpp"

//TODO: this
class Splitter : public Filter {
    std::vector<Filter*> src;
};

#endif
