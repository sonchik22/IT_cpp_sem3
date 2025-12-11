#include <iostream>
#include <random>
#include <chrono>
#include <algorithm> // для std::min

struct subvector {
     int *mas;              //Указатель 
     unsigned int top;      //размер
     unsigned int capacity; //емкость
};

//Интересный факт, Cursor AI автомотически по нажатию 2х кнопок
// создает подобную структуру данных с реализацией всех функций, достаточно написать просто vector и все)
// но я это тестил в прошлом семестре, сейчас временная подписка кончилась :(

bool init(subvector *qv); // инициализация пустого недовектора (top и capacity по нулям, а mas это NULL)
bool push_back(subvector *qv, int d); // добавление элемента в конец недовектора с выделением дополнительной памяти при необходимости
int pop_back(subvector *qv); // удаление элемента с конца недовектора, значение удаленного элемента вернуть (если недовектор пустой, вернуть ноль)
bool resize(subvector *qv, unsigned int new_capacity); // увеличить емкость недовектора (можно использовать и для уменьшения - тогда, в рамках данной реализации, если top меньше новой capacity, то копируем только то, что влезает, и уменьшаем top до capacity)
void shrink_to_fit(subvector *qv); // очистить неиспользуемую память, переехав на новое место с уменьшением capacity до top
void clear(subvector *qv); // очистить содержимое недовектора, занимаемое место при этом не меняется
void destructor(subvector *qv);	// очистить всю используемую память, инициализировать недовектор как пустой


bool init(subvector *qv) {
    qv->mas = NULL;
    qv->top = 0;
    qv->capacity = 0;
    return true;
}

bool resize(subvector *qv, unsigned int new_capacity) {
    if (new_capacity == 0) {        // размер 0 -> всё удаляем
        destructor(qv); 
        return true;
    }

    int *new_mas = new int[new_capacity];

    unsigned int n = qv->top;
    if (new_capacity < n) {         //если новый размер меньше, копируем сколько влезает
        n = new_capacity;
    }

    for (unsigned int i = 0; i < n; i++) {
        new_mas[i] = qv->mas[i];            //вставка за О(n)
    }

    delete[] qv->mas;
    qv->mas = new_mas;
    qv->capacity = new_capacity;

    if (qv->top > new_capacity) {           //если обрезали вектор, то меняем и top
        qv->top = new_capacity;
    }

    return true;
}

bool push_back(subvector *qv, int d) {      
    if (qv->top >= qv->capacity) {          //если места не хватает увеличиваем в 2 раза, или делаем 1 если было 0
        unsigned int new_cap = qv->capacity * 2;
        if (new_cap == 0) new_cap = 1;
        
        if (!resize(qv, new_cap)) {
            return false;
        }
    }

    qv->mas[qv->top] = d;
    qv->top++;
    return true;
}

int pop_back(subvector *qv) {
    if (qv->top == 0) {
        return 0;
    }
    qv->top--;  //тк нумерация идет с 0, а top указывает на колво элементов ->
                // top указывает на первую пустую ячейку (не заню на сколько это безопасно ст точки зрения кибербеза)
    return qv->mas[qv->top];
}

void shrink_to_fit(subvector *qv) { 
    if (qv->capacity > qv->top) {   // если выделено больше памяти, чем реально используется
        resize(qv, qv->top);
    }
}

void clear(subvector *qv) {
    qv->top = 0;
}

void destructor(subvector *qv) {
    delete[] qv->mas;
    qv->mas = NULL;
    qv->top = 0;
    qv->capacity = 0;
}


//ваш код
// тоже с гпт кстати)))

using std::cout;
using std::endl;
double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
}
int rand_uns(int min, int max)
{
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
}
int main()
{
    int n = 100000;
    int *test_sequence = new int[n], sum_for_O3 = 0, sum_check = 0,
        *pop_push_sequence_eq = new int[n],
        *pop_push_sequence_push = new int[n],
        *pop_push_sequence_pushpush = new int[n];
    double start = 0, finish = 0, total = 0;
    cout << std::fixed;
    cout.precision(4);
//----------- Initialization
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        test_sequence[i] = rand_uns(0, n - 1);
        pop_push_sequence_eq[i] = rand_uns(0, 1);
        pop_push_sequence_push[i] = rand_uns(0, 5);
        pop_push_sequence_pushpush[i] = rand_uns(0, 10);
    }
    finish = get_time();
    cout << "Test sequence initialization: \t\t" << finish - start << endl;
    subvector sv;
    init(&sv);
//----------- Test 000 Straight push_back
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        push_back(&sv, test_sequence[i]);
    }
    finish = get_time();
    for (int i = 0; i < n; i++)   //!!! This is a hack to bamboozle the O3 optimization.
        sum_for_O3 += sv.mas[i];  // I might as well use it to test push/pop.
    cout << "000 Straight push_back: \t\t" << finish - start << endl;
    total += finish - start;
//----------- Test 001 Straight pop_back
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        sum_check += pop_back(&sv);
    }
    finish = get_time();
    if (sum_check != sum_for_O3)
    {
        cout <<endl <<"--- !!! Failed push/pop consistency !!! ---" << endl;
        return 0;
    }
    cout << "001 Straight pop_back: \t\t\t" << finish - start << endl;
    total += finish - start;
//----------- Test 002 Straight resize up
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        resize(&sv, i);
    }
    finish = get_time();
    shrink_to_fit(&sv);
    if (sv.capacity)
    {
        cout <<endl <<"--- !!! Failed resize/shrink consistency !!! ---" << endl;
        return 0;
    }
    cout << "002 Straight resize up: \t\t" << finish - start << endl;
    total += finish - start;
//----------- Test 003 Random pop/push equal amount
    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_eq[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();
    clear(&sv);
    shrink_to_fit(&sv);
    if (sv.top)
    {
        cout <<endl <<"--- !!! Falied clear !!! ---" << endl;
        return 0;
    }
    if (sv.capacity)
    {
        cout <<endl <<"--- !!! Falied shrink_to_fit !!! ---" << endl;
        return 0;
    }
    cout << "003 Random pop/push equal amount: \t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;
//----------- Test 004 Random pop/push more push
    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_push[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();
    clear(&sv);
    shrink_to_fit(&sv);
    cout << "004 Random pop/push more push: \t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;
//----------- Test 005 Random pop/push much more push
    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_pushpush[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();
    cout << "005 Random pop/push much more push: \t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;
//----------- End of tests
    destructor(&sv);
    cout << "-----------" << endl <<"Alltests finished, total time: \t" << total << endl;
    delete[] test_sequence;
    delete[] pop_push_sequence_eq;
    delete[] pop_push_sequence_push;
    delete[] pop_push_sequence_pushpush;
    return 0;
}
