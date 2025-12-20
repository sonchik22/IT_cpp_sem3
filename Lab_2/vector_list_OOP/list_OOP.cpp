#include <iostream>
#include <utility> // для std::swap, std::move
#include <random>
#include <chrono>

//лучше сначала читайте вектор, там коментариев больше

template<typename T>
class subforwardlist {
private:

    struct Node {
        T data;
        Node* next;
        //конструктор
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* begin; //указатель на начало списка
    unsigned int list_size;

    //метод для поиска узла перед нужным индексом
// это абсолютно правильное решение -- сделать отдельную функцию такую
    Node* get_node_before(unsigned int index) const {
        if (index == 0 || !begin) return nullptr;
        Node* curr = begin;
        for (unsigned int i = 0; i < index - 1; ++i) {
            if (!curr->next) return nullptr; // Индекс за границами
            curr = curr->next;
        }
        return curr;
    }

        void clear_internal() {
        while (begin) {
            Node* temp = begin;
            begin = begin->next;
            delete temp;
        }
        list_size = 0;
    }

public:

    subforwardlist() : begin(nullptr), list_size(0) {}

    ~subforwardlist() {
        clear_internal();
    }

    void clear() {
        clear_internal();
    }

    //конструктор копирования
    subforwardlist(const subforwardlist& other) : begin(nullptr), list_size(0) {
        if (!other.begin) return;

        // Копируем голову
        begin = new Node(other.begin->data);
        list_size++;

        Node* my_curr = begin;
        Node* other_curr = other.begin->next;

        // Копируем хвост
        while (other_curr) {
            my_curr->next = new Node(other_curr->data);
            my_curr = my_curr->next;
            other_curr = other_curr->next;
            list_size++;
        }
    }

    //оператор присваивания копированием
    subforwardlist& operator=(const subforwardlist& other) {
        if (this != &other) {

            subforwardlist temp(other);
// Комментарий аналогично вектору по поводу функции swap
            std::swap(begin, temp.begin);
            std::swap(list_size, temp.list_size);
            // temp уничтожится и заберет наши старые узлы
        }
        return *this;
    }

    //конструктор перемещения
    subforwardlist(subforwardlist&& other) noexcept 
        : begin(other.begin), list_size(other.list_size) 
    {
        other.begin = nullptr;
        other.list_size = 0;
    }

    //Оператор присваивания перемещением
    subforwardlist& operator=(subforwardlist&& other) noexcept {
        if (this != &other) {
            clear_internal(); // Очищаем себя
// Комментарий аналогично вектору по поводу функции swap и в целом логики метода
            // Крадем данные
            begin = other.begin;
            list_size = other.list_size;

            // Зануляем other
            other.begin = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    void push_forward(const T& data) {
        begin = new Node(data, begin);
        list_size++;
    }

    T pop_forward() {
        if (!begin) return T{};

        Node* temp = begin;
        T data = std::move(temp->data); //перемещаем данные перед удалением
        begin = begin->next;
        delete temp;
        
        list_size--;
        return data;
    }

    void push_back(const T& data) {
        if (!begin) {
            push_forward(data);
            return;
        }

        Node* curr = begin;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new Node(data);
        list_size++;
    }

    T pop_back() {
        if (!begin) return T{};

        if (!begin->next) {             //если один элемент
            return pop_forward();
        }

        //ищем предпоследний
        Node* curr = begin;
        while (curr->next->next) {
            curr = curr->next;
        }

        Node* to_del = curr->next;
        T data = std::move(to_del->data);
        delete to_del;
        curr->next = nullptr;
        
        list_size--;
        return data;
    }

    void push_where(unsigned int where, const T& data) {
        if (where == 0) {
            push_forward(data);
            return;
        }

        Node* prev = get_node_before(where); //ищем узел с индексом (where - 1)
        if (!prev) {
            if (where == list_size) push_back(data);
            return; 
        }

        Node* new_node = new Node(data, prev->next);
        prev->next = new_node;
        list_size++;
    }

    T erase_where(unsigned int where) {
        if (!begin) return T{};
        if (where == 0) {
            return pop_forward();
        }

        Node* prev = get_node_before(where);
        if (!prev || !prev->next) return T{}; //нечего удалять

        Node* to_del = prev->next;
        T data = std::move(to_del->data);
        prev->next = to_del->next;
        delete to_del;
        
        list_size--;
        return data;
    }

    unsigned int size() {
        return list_size;
    }
};

// код здесь
