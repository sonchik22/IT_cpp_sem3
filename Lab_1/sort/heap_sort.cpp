#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>   //время
#include <iomanip>  //красивый вывод
#include <cmath>
#include <cstdlib>  //рандомайзер
#include <ctime>    // time() чтобы быстро получить текущее время

void heapify(int*** array, int n, int i) {      // сами сказали поставить звезд сколько хотим)))
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && (**array)[left] > (**array)[largest])
            largest = left;

        if (right < n && (**array)[right] > (**array)[largest])
            largest = right;

        if (largest == i)
            break;

        std::swap((**array)[i], (**array)[largest]);
        i = largest;
    }
}

void my_sort(int n, int*** array) {
    // парсим кучу, чтобы максимальный элемент был в начале
    for (int i = n / 2 - 1; i >= 0; i--)    // от середины массива к началу
        heapify(array, n, i);

    // сортируес
    for (int i = n - 1; i > 0; i--) {
        std::swap((**array)[0], (**array)[i]);
        heapify(array, i, 0);
    }
}

bool is_sorted(int* array, int n) {
    for (int i = 0; i < n - 1; ++i) {
        if (array[i] > array[i + 1]) return false;
    }
    return true;
}

int main() {
    // инициализация генератора случайных чисел
    srand(time(0)); 

    // размеры массивов для теста (увеличиваем каждый раз в 2 раза)
    std::vector<int> sizes = {10000, 20000, 40000, 80000, 160000, 320000, 640000, 1280000};

    std::cout << std::setw(15) << "N (Elements)" 
              << std::setw(20) << "Time (seconds)" 
              << std::setw(15) << "Sorted?" 
              << std::setw(20) << "Ratio T/(N*logN)" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (int n : sizes) {
        std::vector<int> data(n);
        
        // генерация чисел
        for (int i = 0; i < n; ++i) {
            // rand() дает число. 
            // % 100000 ограничивает диапазон от 0 до 99999.
            data[i] = rand() % 100000; 
        }

        int* ptr1 = &data[0];   //адрес начала массива
        int** ptr2 = &ptr1;
        int*** ptr3 = &ptr2;

        auto start = std::chrono::high_resolution_clock::now(); //запуск отсчета. auto ставим потому что
                                                                // лень думать над типом возвращаемых данных,
                                                                // поэтому компилятор сам подставит
        my_sort(n, ptr3);                                       //считаем
        auto end = std::chrono::high_resolution_clock::now();   //конец отсчета
        
        std::chrono::duration<double> diff = end - start;       //гпт сказал что тут тип auto не подойдет тк
                                                                // тут с помощью chrono::duration мы автоматтически 
                                                                // переводим все в секунды, а если напишем
                                                                //auto diff = ... то останется в нс/мкс/тактах 
                                                                //или хз вообще в чем
        bool sorted = is_sorted(&data[0], n);
        double ratio = diff.count() / (n * std::log2(n));       //diff.count() достает из duration само число секунд
                                                                //n * std::log2(n) - количество операци

        //красивая табличка
        std::cout << std::setw(15) << n 
                  << std::setw(20) << std::fixed << std::setprecision(6) << diff.count() 
                  << std::setw(15) << (sorted ? "Yes" : "No") 
                  << std::setw(20) << std::scientific << ratio << std::endl;
    }

    std::cout << "\n avg raito T/(N*logN) = 3";

    return 0;
}
