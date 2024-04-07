
#include <iostream>

class counter {
public:
    int n = 0;
    int operator()(int a, int b, int c, int d) {
        return n++;
    }
};

std::ostream& operator<<(std::ostream& s, counter& c){
    s << c.n++;
    return s;
}

int main() {
    counter c;
    for(int i = 0; i < 10; i++) {
        std::cout << c << std::endl;;
    }
}

/* ip = "   ==    "
 * ip.find_first_of() returns 4
 * ip.find_last_of() returns 5
 */
