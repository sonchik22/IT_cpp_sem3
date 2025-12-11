#include <iostream>
#include <cmath>

int main() {
    float a = 8388608.0f;  // 2^23
    float b = a + 0.55f;
    std::cout << "a = " << a << std::endl;  // 8388608
    std::cout << "b = " << b << std::endl;  // 8388609
    std::cout << "b - a = " << (b - a) << std::endl;  // 1
    std::cout << "b == a + 1? " << ((b == a + 1.0f) ? "True":"False") << std::endl;  // true
    return 0;
}
// интересный факт, если b = a + x, х <1.5, то результат будет true(для а и б из примера)
// х_0, где х > x_0 такой что x == х + 1 будет лежать в интервале [2^24; +inf)