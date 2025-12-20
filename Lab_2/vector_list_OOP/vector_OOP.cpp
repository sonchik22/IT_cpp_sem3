#include <iostream>
#include <algorithm> // для std::swap, std::min
#include <utility>   // для std::move
#include <random>
#include <chrono>

template<typename T>
class subvector {
public:
    T* mas;                 //Указатель 
    unsigned int top;       //размер
    unsigned int capacity;  //емкость

    // bool push_back(const T& d); // добавление элемента в конец недовектора с выделением дополнительной памяти при необходимости
    // T pop_back(); // удаление элемента с конца недовектора, значение удаленного элемента вернуть (если недовектор пустой, вернуть T{})
    // bool resize( unsigned int new_capacity); // увеличить емкость недовектора 
    // (можно использовать и для уменьшения - тогда, в рамках данной реализации, 
    // если top меньше новой capacity, то копируем только то, что влезает, и уменьшаем top до capacity)
    // void shrink_to_fit(); // очистить неиспользуемую память, переехав на новое место с уменьшением capacity до top
    // void clear(); // очистить содержимое недовектора, занимаемое место при этом не меняется

    //конструктор
    subvector() : mas(nullptr), top(0), capacity(0) {}
    //деструктор
    ~subvector() {
        delete[] mas;
    }
    //правило пяти:
    //1.Деструктор (~Class()): (освободи память).
    //2.Конструктор копирования (Class(const Class&)): (выдели новую память и скопируй данные).
    //3.Оператор присваивания копированием (operator=(const Class&)): (очисти себя, выдели новую память, скопируй данные).
    //4.Конструктор перемещения (Class(Class&&)): "Забери душу умирающего" (забери указатель у временного объекта, а его указатель занули).
    //5.Оператор присваивания перемещением (operator=(Class&&)): "Забери ресурсы у умирающего соседа" (очисти себя, забери указатель у временного объекта, занули его).
    //конструктор копирования
    subvector(const subvector& other) 
        : mas(nullptr), top(other.top), capacity(other.capacity) 
    {
        if (other.capacity > 0) {
            mas = new T[other.capacity];
            for (unsigned int i = 0; i < top; ++i) {
                mas[i] = other.mas[i];
            }
        }
    }
// Тут кстати можно было бы сделать memcpy, тогда не было бы цикла в теле функции

    //присваивание копированием
    subvector& operator=(const subvector& other) {
        if (this != &other) { //от самоприсваивания
            subvector temp(other);
// Три строчки ниже можно вынести в отдельную функцию и использовать её так же и в move ctor 
            std::swap(mas, temp.mas);
            std::swap(top, temp.top);
            std::swap(capacity, temp.capacity);
            // temp уничтожится при выходе из функции
        }
        return *this;
    }

    //конструктор перемещения
    //noexcept дает гарантию времени компиляции, что из функции не будет выброшено исключение.
    subvector(subvector&& other) noexcept
        : mas(other.mas), top(other.top), capacity(other.capacity) //тут все копируем
    {
        //тут очищаем исходники
        other.mas = nullptr;
        other.top = 0;
        other.capacity = 0;
    }

    //присваивание перемещением
// Тут как раз работает чисто swap. Не обязательно занулять состояние other, можно отдать ему своё состояние, тогда как раз поможем функция swap из комментария выше
    subvector& operator=(subvector&& other) noexcept {
        if (this != &other) {
            delete[] mas;

            mas = other.mas;
            top = other.top;
            capacity = other.capacity;

            other.mas = nullptr;
            other.top = 0;
            other.capacity = 0;
        }
        return *this;
    }


    bool resize(unsigned int new_capacity) {
        if (new_capacity == 0) {
            if (mas) delete[] mas;
            mas = nullptr;
            top = 0;
            capacity = 0;
            return true;
        }

        if (new_capacity == capacity) {
            return true;
        }

        T* new_mas = new T[new_capacity];


        unsigned int n = top;
        if (new_capacity < n) {     //если новый размер меньше, копируем сколько влезает
            n = new_capacity;
        }

        for (unsigned int i = 0; i < n; i++) {
            new_mas[i] = mas[i];
        }

        if (mas) delete[] mas;
        
        mas = new_mas;
        capacity = new_capacity;

        if (top > new_capacity) {       //если обрезали вектор, то меняем и top
            top = new_capacity;
        }

        return true;
    }


    bool push_back(const T& d) {
        if (top >= capacity) {
            unsigned int new_cap = (capacity == 0) ? 1 : capacity * 2;
            if (!resize(new_cap)) {
                return false;
            }
        }
        mas[top] = d;
        top++;
        return true;
    }

    T pop_back() {
        if (top == 0) {
            return T{};
        }
        top--;
        return mas[top];
    }

    void shrink_to_fit() {
        if (top < capacity) {
            resize(top);
        }
    }

    void clear() {
        top = 0;
    }
};

