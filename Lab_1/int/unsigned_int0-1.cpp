#include <iostream>
#include <limits>
#include <cstdint>

void print_bits_unsigned(unsigned int x) {
    unsigned int mask = 1u << 31;
    for (int i = 0; i < 32; ++i) {
        std::cout << ((x & mask) ? '1' : '0');
        mask >>= 1;
        if ((i + 1) % 4 == 0) std::cout << ' ';
    }
}

int main() {
    unsigned int zero = 0;
    std::cout << "0u: ";
    print_bits_unsigned(zero);
    std::cout << " (" << zero << ")" << std::endl;
    // Вывод: 00000000 00000000 00000000 00000000 (значение: 0)
    unsigned int underflow = zero - 1;
    std::cout << "0u - 1u: ";
    print_bits_unsigned(underflow);
    std::cout << " (" << underflow << ")" << std::endl;
    // Вывод: 11111111 11111111 11111111 11111111 (значение: 4294967295)
    // потому что вычитание 1 из 0 это то же самое что заимствование, поэтому происходит инвертирование 0 в 1
    return 0;
}
