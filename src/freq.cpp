#include "freq.hpp"


std::istream& operator>>(std::istream& str, Frequency& f) {
    std::stringstream tex{};
    str >> skipws;

    char c;
    if(!isdigit(str.peek())) {
        for(int i = 0; i < 2 && str.get(c); i++) {
            tex << c;
        }

        if(isNoteMod(c) && str.get(c))
            tex << c;
    }
    //this one is cursed...
    while(isdigit(str.peek()) && str.get(c)) {
        tex << c;
    }

    f = Frequency(tex);
    return str;
}

std::ostream& operator<<(std::ostream& str, Frequency& f) { return str << f.getName(); }
