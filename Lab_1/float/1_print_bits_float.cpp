#include <iostream>
#include <cstdint>

void print_bits_float(float x) {
    union FloatUnion {
        float f;
        unsigned int bits;
    };
    FloatUnion u;
    u.f = x;
    unsigned int bits = u.bits;
    
    std::cout << "S (1 b): " << ((bits >> 31) & 1) << " ";  // тк по IEEE 754 нумерация float идет слева на право
    std::cout << "E (8 b): ";
    for (int i = 0; i < 8; i++) {
        std::cout << ((bits >> (30 - i)) & 1);
    }
    std::cout << " M (23 b): ";
    for (int i = 22; i >= 0; --i) {
        std::cout << ((bits >> i) & 1);
        if (i % 4 == 0 && i > 0) std::cout << ' ';
    }
    std::cout << std::endl;
}

int main() {
    float f = -3.5f;
    std::cout << f << ":" << std::endl;
    print_bits_float(f);
    return 0;
}
/*
Значение вычисляется как (-1)^S * (1.M) * 2^(E - 127)
пример:
-3,5 = -1 * (1.75) * 2^1 => S = 1, M = 0.75 = 1/2 + 1/4 = 110...00...0, E = 10000000


*/
