
#include <iostream>

template<typename T>
class subvector {
    T* mas;
    unsigned int top;
    unsigned int capacity;

public:
    subvector() {
        mas = nullptr;
        top = 0;
        capacity = 0;
    }

    ~subvector() {
        if (mas) delete[] mas;
    }

    // Конструктор копирования
    subvector(const subvector& other) {
        top = other.top;
        capacity = other.capacity;
        if (capacity == 0) {
            mas = nullptr;
        } else {
            mas = new T[capacity];
            for (unsigned int i = 0; i < top; ++i) mas[i] = other.mas[i];
        }
    }

    // Оператор присваивания
    subvector& operator=(const subvector& other) {
        if (this == &other) return *this;
        if (mas) delete[] mas;

        top = other.top;
        capacity = other.capacity;
        if (capacity == 0) {
            mas = nullptr;
        } else {
            mas = new T[capacity];
            for (unsigned int i = 0; i < top; ++i) mas[i] = other.mas[i];
        }
        return *this;
    }

    // Конструктор перемещения
    subvector(subvector&& other) noexcept {
        mas = other.mas;
        top = other.top;
        capacity = other.capacity;
        other.mas = nullptr; other.top = 0; other.capacity = 0;
    }

    // Оператор присваивания перемещением
    subvector& operator=(subvector&& other) noexcept {
        if (this == &other) return *this;
        if (mas) delete[] mas;
        mas = other.mas;
        top = other.top;
        capacity = other.capacity;
        other.mas = nullptr; other.top = 0; other.capacity = 0;
        return *this;
    }

    bool resize(unsigned int new_capacity) {
        if (new_capacity == 0) {
            if (mas) delete[] mas;
            mas = nullptr; top = 0; capacity = 0;
            return true;
        }
        
        if (new_capacity == capacity) return true;

        T* new_mas = new T[new_capacity];
        
        unsigned int n = (top < new_capacity) ? top : new_capacity;
        for (unsigned int i = 0; i < n; i++) {
            new_mas[i] = mas[i];
        }

        if (mas) delete[] mas;
        mas = new_mas;
        capacity = new_capacity;
        if (top > capacity) top = capacity;
        
        return true;
    }

    bool push_back(const T& d) {
        if (top >= capacity) {
            unsigned int new_cap = (capacity == 0) ? 1 : capacity * 2;
            if (!resize(new_cap)) return false;
        }
        mas[top++] = d;
        return true;
    }

    T pop_back() {
        if (top > 0) return mas[--top];
        return T();
    }


    T& operator[](unsigned int index) {
        return mas[index];
    }

    const T& operator[](unsigned int index) const {
        return mas[index];
    }

    unsigned int size() const {
        return top;
    }

    // Метод для инициализации вектора конкретным размером и значением
    void resize_fill(unsigned int new_size, const T& value) {

        if (new_size > capacity) {
            resize(new_size); 
        }

        top = new_size;

        for(unsigned int i = 0; i < top; ++i) {
            mas[i] = value;
        }
    }
};

