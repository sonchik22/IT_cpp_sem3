#include <iostream>
#include <limits>
#include <cstdint>

void print_bits_int(int x) {
    unsigned int mask = 1u << 31;
    for (int i = 0; i < 32; ++i) {
        std::cout << ((x & mask) ? '1' : '0');
        mask >>= 1;
        if ((i + 1) % 4 == 0) std::cout << ' ';
    }
}

int main() {
    int max_val = std::numeric_limits<int>::max();  // INT_MAX = 2147483647
    std::cout << "INT_MAX: ";
    print_bits_int(max_val);
    std::cout << " (" << max_val << ")" << std::endl;
    // Вывод: 01111111 11111111 11111111 11111111 (значение: 2147483647)

    int overflow = max_val + 1;
    std::cout << "INT_MAX + 1: ";
    print_bits_int(overflow);
    std::cout << " (" << overflow << ")" << std::endl;
    // Вывод: 10000000 00000000 00000000 00000000 (значение: -2147483648)
    // Такой результат потому что происходит перенос а старший бит отвечает за знак
    return 0;
}
