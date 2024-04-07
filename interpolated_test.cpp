
#include "interpolated.hpp"

#define INTERPOLATED_TEST

#ifdef INTERPOLATED_TEST
int main() {
    std::vector<std::pair<float, float>> subject1 = {{1, 2}, {3, 4}, {5, 6}};
    Interpolated<float> test(subject1);
    std::cout << "Result 1 :" << test.Get(1.5f) << std::endl << "Excepted: 2.5" << std::endl;
    std::cout << "Result 2 :" << test.Get(-1) << std::endl << "Excepted: 2" << std::endl;
    std::cout << "Result 3 :" << test.Get(6) << std::endl << "Excepted: 5" << std::endl;
}
#endif

