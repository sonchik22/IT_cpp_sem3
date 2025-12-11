#include <iostream>
#include <iomanip>

int main() {
    float sum = 0.0f;
    float prev = 0.0f;
    int k = 1;
    int maybe_stop = 0;
    while (true) {
        prev = sum;
        sum += 1.0f / k;
        if (sum == prev) {
            //Добавление не увеличивает сумму
            if (k == maybe_stop * 100)      //вот тут константу меняем и видим что одинаково при всех к после 2097152
            {                               //тк округление
                break;
            }
            k++;
            continue;
        }
        k++;
        maybe_stop = k;
    }
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "sum stab on k = " << k << std::endl;
    std::cout << "total amount = " << sum << std::endl;
    return 0;
}
