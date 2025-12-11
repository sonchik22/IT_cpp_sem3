#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    int k = 2097152;              // Значение, после которого сумма стабилизируется
    int N = 100;
    std::vector<float> terms;
    for (int i = 0; i <= N; ++i) {
        terms.push_back(1.0f / (k + i));
    }

    //в прямом
    float sum_forward = 0.0f;
    for (int i = 0; i <= N; ++i) {
        sum_forward += terms[i];
    }

    //в обратном
    float sum_reverse = 0.0f;
    for (int i = N; i >= 0; --i) {
        sum_reverse += terms[i];
    }

    std::cout << std::setprecision(15);
    std::cout << "sum_forward: " << sum_forward << std::endl;
    std::cout << "sum_reverse: " << sum_reverse << std::endl;
    std::cout << "sub: " << sum_forward - sum_reverse << std::endl;

    return 0;
}
/*
из ассоциотивности поняли что нужно складывать числа одного порядка

тут в первом случае мы большое складваем с меньшим доходим то предела округления
и очень маленькие числа округляются и не вносят вклад.

во втором случае мы маленькое число складываем с числом немного больше
(но все еще маленьким), в результате порядок суммируемого числа на каждой итеррации
растет и точность становится выше.
*/