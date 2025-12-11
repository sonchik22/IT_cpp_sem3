#include <stdio.h>

void print_bits_int(int x) {
    unsigned int mask = 1u << 31;
    for (int i = 0; i < 32; ++i) {
        putchar((x & mask) ? '1' : '0');
        mask >>= 1;
        if ((i + 1) % 4 == 0) putchar(' ');
    }
}

int main() {
    int c = -13;
    int d = 4294967283;
    unsigned int f = (unsigned)c;

    printf("signed c  = ");
    print_bits_int(c);
    printf("c = %d\n", c);

    printf("unsigned c  = ");
    print_bits_int(((unsigned)c));
    printf("c = %u\n", f);

    return 0;
}
/*
Действительно ли unsigned int "сдвинут"на половину диапазона?
-Да в 2 раза больше чисел можем записать так как первый бит теперь не знаковый, а значащий
*/

