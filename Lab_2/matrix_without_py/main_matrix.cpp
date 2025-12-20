#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib> // для rand
#include <ctime>   // для time
#include "vector_for_matrix.cpp"

template<typename T>
class Matrix {
    subvector<T> data;
    unsigned n_rows, n_cols;

public:
    Matrix(unsigned rows, unsigned cols, T value = T()) : n_rows(rows), n_cols(cols) {
        data.resize_fill(rows * cols, value);
    }

    static Matrix Identity(unsigned rows, unsigned cols) {
        Matrix m(rows, cols, 0);
        for (unsigned i = 0; i < rows; ++i) {
            m(i, i) = 1;
        }
        return m;
    }

    static Matrix getSpecificDeterminant(unsigned n, T determinant) {
        Matrix m = Matrix::Identity(n, n);
        
        //нужный детерминант в угле
        m(0, 0) = determinant;

        // Запутываем (добавляем строки друг к другу)
        // R1 = R1 + k * R2
        int iterations = n * 2; // количество запутываний
        for (int k = 0; k < iterations; ++k) {
            unsigned r1 = rand() % n;
            unsigned r2 = rand() % n;
            if (r1 == r2) continue;

            T factor = (T)((rand() % 100) / 10.0 - 5.0); // число от -5.0 до 5.0
            // Добавление одной строки к другой хорошо было бы сделать отдельным методом, так как он ещё понадобится в расчете дтерминанта
            for (unsigned c = 0; c < n; ++c) {
                m(r1, c) += factor * m(r2, c);
            }
        }
        return m;
    }

    unsigned rows() const { return n_rows; }
    unsigned cols() const { return n_cols; }

    Matrix transpose() const {
        Matrix res(n_cols, n_rows);
        for (unsigned r = 0; r < n_rows; ++r) {
            for (unsigned c = 0; c < n_cols; ++c) {
                res(c, r) = (*this)(r, c);
            }
        }
        return res;
    }

    //оператор доступа неконстантный
    T& operator()(unsigned row, unsigned col) {
        return data[row * n_cols + col];
    }

    // оператор доступа константный
    T operator()(unsigned row, unsigned col) const {
        return data[row * n_cols + col];
    }

    void swap_rows(unsigned r1, unsigned r2) {
        if (r1 == r2) return;
        for (unsigned c = 0; c < n_cols; ++c) {
            // лучше std::swap вместо swap руками, потому что не учтён std::move
            T temp = (*this)(r1, c);
            (*this)(r1, c) = (*this)(r2, c);
            (*this)(r2, c) = temp;
        }
    }
};

template<typename T>
T determinant(const Matrix<T>& mat) {
    if (mat.rows() != mat.cols()) return 0;
    
    unsigned n = mat.rows();
    Matrix<T> m = mat; //копия
    T det = 1;
    const double EPS = 1e-9;

    for (unsigned i = 0; i < n; ++i) {
        unsigned pivot = i;
        for (unsigned j = i + 1; j < n; ++j) {
            if (std::abs(m(j, i)) > std::abs(m(pivot, i))) {
                pivot = j;
            }
        }

        if (std::abs(m(pivot, i)) < EPS) return 0;

        if (pivot != i) {
            m.swap_rows(i, pivot);
            det = -det;
        }
        
        det *= m(i, i);

        for (unsigned j = i + 1; j < n; ++j) {
            if (std::abs(m(j, i)) > EPS) {
                // и вот тут как раз этот метод пригодился бы
                T factor = m(j, i) / m(i, i);
                for (unsigned k = i; k < n; ++k) {
                    m(j, k) -= factor * m(i, k);
                }
            }
        }
    }

    return det;
}

void run_test(unsigned size, double target_det) {
    std::cout << "Running test " << size << "x" << size 
              << " with expected det = " << target_det << "..." << std::endl;

    Matrix<double> m = Matrix<double>::getSpecificDeterminant(size, target_det);
    double calc_det = determinant(m);
    
    //транспонированная
    Matrix<double> m_t = m.transpose();
    double calc_det_t = determinant(m_t);

    std::cout << "  Calculated: " << calc_det << std::endl;
    std::cout << "  Transposed: " << calc_det_t << std::endl;
    // Зачем же такая грубая проверка. Только сегодня обсуждали, что нужно флоаты проверять на равенство через машинное эпсилон
    if (std::abs(calc_det - target_det) < 1.0) { //грубая проверка
        std::cout << "  [OK]" << std::endl;
    } else {
        std::cout << "  [FAIL]" << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void test_manual_matrix() {
    std::cout << "MANUAL test (5x5, Expected Det = 465)..." << std::endl;

    Matrix<double> m(5, 5);

    double raw_data[] = {
         3,  -1,  2,   -1,  1,
         5,   1,    -2,     1,    2,
         9,   -1,    1,     3,    4,
         3,   0,     6,     -1,     3,
         5,    2,    3,    -2,    1
    };

    int k = 0;
    for (unsigned i = 0; i < 5; ++i) {
        for (unsigned j = 0; j < 5; ++j) {
            m(i, j) = raw_data[k++];
        }
    }

    double det = determinant(m);

    std::cout << "  Calculated Det: " << det << std::endl;

    if (std::abs(static_cast<int>(det)) == 465) { 
        std::cout << "  [OK] Determinant is 465" << std::endl;
    } else {
        std::cout << "  [FAIL] Determinant is NOT 465" << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}


int main() {
    srand(time(0));

    test_manual_matrix();   
    run_test(5, 120.0);
    run_test(50, 500.0);
    run_test(100, -10.0);

    return 0;
    
}
