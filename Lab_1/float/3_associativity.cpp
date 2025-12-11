#include <iostream>
#include <iomanip>  // Для std::setprecision

int main() {
    float a = 1.0f;
    float b = 1e30f;
    float c = -1e30f;

    float left = (a + b) + c;
    float right = a + (b + c);
    
    std::cout << "(a + b) + c = " << left << std::endl;
    std::cout << "a + (b + c) = " << right << std::endl;
    std::cout << "equel? " << (left == right ? "True" : "False") << std::endl;

    return 0;
}
