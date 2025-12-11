#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cmath>
//#include <cassert>
#include <cstdlib> 
#include <ctime>   

/*
принимаем указатель array.
Левая часть: от array[0] до array[mid-1]
Правая часть: от array[mid] до array[n-1]
*/
void custom_merge(int* array, int left_len, int right_len) {
    int total_len = left_len + right_len;
    //временный буфер
    std::vector<int> temp(total_len);

    int i = 0; //индекс для левой части
    int j = 0; // для правой части
    int k = 0; // для временного массива
    
    // Указатель на начало правой части
    int* left_part = array;

    int* right_part = array + left_len;

    // пока оба индекса внутри своих диапазонов
    while (i < left_len && j < right_len) {
        if (left_part[i] <= right_part[j]) {
            temp[k] = left_part[i];
            i++;
        } else {
            temp[k] = right_part[j];
            j++;
        }
        k++;
    }

    //если в левой части что-то осталось
    while (i < left_len) {
        temp[k] = left_part[i];
        i++;
        k++;
    }

    //аналогично
    while (j < right_len) {
        temp[k] = right_part[j];
        j++;
        k++;
    }

    // копируем из временного массива в основной
    for (int m = 0; m < total_len; ++m) {
        array[m] = temp[m];
    }
}

void my_sort(int n, int* array) {

    if (n <= 1) {
        return;
    }

    int mid = n / 2; // Находим середину

    // разделение для левой половины
    my_sort(mid, array);

    // резделение для правой половины
    my_sort(n - mid, array + mid);

    //слияние 
    custom_merge(array, mid, n - mid);
}

bool is_sorted(int* array, int n) {
    for (int i = 0; i < n - 1; ++i) {
        if (array[i] > array[i + 1]) return false;
    }
    return true;
}

int main() {
    srand(time(0)); 

    // Размеры массивов для теста
    std::vector<int> sizes = {10000, 20000, 40000, 80000, 160000, 320000, 640000, 1280000};

    std::cout << std::setw(15) << "N (Elements)" 
              << std::setw(20) << "Time (seconds)" 
              << std::setw(15) << "Sorted?" 
              << std::setw(20) << "Ratio T/(N*logN)" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (int n : sizes) {

        std::vector<int> data(n);
        for (int i = 0; i < n; ++i) {
            data[i] = rand() % 100000; 
        }

        auto start = std::chrono::high_resolution_clock::now();
        my_sort(n, &data[0]);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> diff = end - start;
        bool sorted = is_sorted(&data[0], n);

        double ratio = diff.count() / (n * std::log2(n));

        std::cout << std::setw(15) << n 
                  << std::setw(20) << std::fixed << std::setprecision(6) << diff.count() 
                  << std::setw(15) << (sorted ? "Yes" : "No") 
                  << std::setw(20) << std::scientific << ratio << std::endl;
    }

        std::cout << "\n avg raito T/(N*logN) = 4";

    return 0;
}
