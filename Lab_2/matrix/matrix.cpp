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
        unsigned min_dim = (rows < cols) ? rows : cols;
        for (unsigned i = 0; i < min_dim; ++i) {
            m(i, i) = 1;
        }
        return m;
    }

    static Matrix getSpecificDeterminant(unsigned n, T determinant) {

        Matrix m = Matrix::Identity(n, n);
        
        // ставим нужный детерминант в угол
        m(0, 0) = determinant;

        // добавляем строки друг к другу, чтобы запутсть
        // R1 = R1 + k * R2
        int iterations = n * 2; // количество запутываний
        for (int k = 0; k < iterations; ++k) {
            unsigned r1 = rand() % n;
            unsigned r2 = rand() % n;
            if (r1 == r2) continue;

            T factor = (T)((rand() % 100) / 10.0 - 5.0); // число от -5.0 до 5.0
            
            for (unsigned c = 0; c < n; ++c) {
                m(r1, c) += factor * m(r2, c);
            }
        }
        return m;
    }

    unsigned rows() const { return n_rows; }
    unsigned cols() const { return n_cols; }

    //создает копию и меняет ее
    Matrix transpose() const {
        Matrix res(n_cols, n_rows);
        for (unsigned r = 0; r < n_rows; ++r) {
            for (unsigned c = 0; c < n_cols; ++c) {
                res(c, r) = (*this)(r, c);
            }
        }
        return res;
    }

    //меняет себя
    Matrix& transpose() {
        for (unsigned r = 0; r < n_rows; ++r) {
            for (unsigned c = r + 1; c < n_cols; ++c) {
                T temp = (*this)(r, c);
                (*this)(r, c) = (*this)(c, r);
                (*this)(c, r) = temp;
            }
        }
        
        return *this;
    }

    // для записи
    T& operator()(unsigned row, unsigned col) {
        return data[row * n_cols + col];
    }

    // рид онли
    T operator()(unsigned row, unsigned col) const {
        return data[row * n_cols + col];
    }

    void swap_rows(unsigned r1, unsigned r2) {
        if (r1 == r2) return;
        for (unsigned c = 0; c < n_cols; ++c) {
            T temp = (*this)(r1, c);
            (*this)(r1, c) = (*this)(r2, c);
            (*this)(r2, c) = temp;
        }
    }

    T determinant() const {
        // Проверка, что матрица квадратная
        if (n_rows != n_cols) return 0;
        
        unsigned n = n_rows;

        Matrix<T> temp_mat = *this; 
        
        T det = 1;
        const double EPS = 1e-9; // Порог для сравнения с нулем (для double)

        for (unsigned i = 0; i < n; ++i) {  //смотрим на iй столбец и iюу строку 
            //находим в столбце i наибольший элемент
            unsigned pivot = i;
            for (unsigned j = i + 1; j < n; ++j) {
                if (std::abs(temp_mat(j, i)) > std::abs(temp_mat(pivot, i))) {
                    pivot = j;
                }
            }

            //если наибольший элемент 0 значит вся строка 0 значит определитель 0
            if (std::abs(temp_mat(pivot, i)) < EPS) return 0;

            //если наибольший стоит не на диагонали i,i , то ставим его на диагональ
            if (pivot != i) {
                temp_mat.swap_rows(i, pivot);
                det = -det; // при перестановке знак меняется
            }

            //домножаем определитель на число из диагонали
            det *= temp_mat(i, i);

            // зануляем столбец под текущим элементом
            for (unsigned j = i + 1; j < n; ++j) {
                if (std::abs(temp_mat(j, i)) > EPS) {
                    T factor = temp_mat(j, i) / temp_mat(i, i);
                    for (unsigned k = i; k < n; ++k) {
                        temp_mat(j, k) -= factor * temp_mat(i, k);
                    }
                }
            }
        }
        return det;
    }
};