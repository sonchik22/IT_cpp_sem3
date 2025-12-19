#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>  // for setprecision

#include <pybind11/embed.h> 
#include <pybind11/stl.h>

#include "vector_for_matrix.cpp"
#include "matrix.cpp"

namespace py = pybind11;

template <typename T>
Matrix<T> convert_py_to_cpp(const std::vector<std::vector<double>>& py_data) {
    unsigned rows = py_data.size();
    if (rows == 0) return Matrix<T>(0, 0);
    unsigned cols = py_data[0].size();
    Matrix<T> m(rows, cols);
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) m(r, c) = static_cast<T>(py_data[r][c]);
    }
    return m;
}

template <typename T>
void print_matrix_preview(const Matrix<T>& m) {
    unsigned r = m.rows();
    unsigned c = m.cols();
    std::cout << "   Matrix (" << r << "x" << c << "):\n";

    // если матрица маленькая, печатаем 
    if (r <= 10 && c <= 10) {
        for (unsigned i = 0; i < r; ++i) {
            std::cout << "   | ";
            for (unsigned j = 0; j < c; ++j) {
                std::cout << std::setw(6) << m(i, j) << " ";
            }
            std::cout << "|\n";
        }
    } 
    
    std::cout << std::endl;
}

void run_test(py::object& py_func, int size) {
    std::cout << "TEST SIZE: " << size << "x" << size << "\n";

    //генерируем матрицу в Python
    auto result = py_func(size).cast<std::pair<std::vector<std::vector<double>>, double>>();
    Matrix<double> m = convert_py_to_cpp<double>(result.first);
    double expected = result.second;

    print_matrix_preview(m);


    double calculated = m.determinant();

    //для транспонированной
    Matrix<double> m_transposed = m.transpose();
    double det_transposed = m_transposed.determinant();
    
    std::cout << std::setprecision(5);
    std::cout << " > Numpy Det:        " << expected << "\n";
    std::cout << " > My Det (Original):  " << calculated << "\n";
    std::cout << " > My Det (Transposed):" << det_transposed << "\n";

    double tolerance = 1e-4;
    if (std::abs(expected) > 1e10) tolerance = std::abs(expected) * 1e-9;       //для больших чисел

    bool match_numpy = std::abs(calculated - expected) < tolerance;
    bool match_transpose = std::abs(calculated - det_transposed) < tolerance;

    if (match_numpy) std::cout << " [OK] Matches Numpy.\n";
    else             std::cout << " [FAIL] Diff with Numpy: " << std::abs(calculated - expected) << "\n";

    if (match_transpose) std::cout << " [OK] Transpose property holds (Det(A) == Det(A^T)).\n";
    else                 std::cout << " [FAIL] Transpose property broken!\n";

    std::cout << "\n";
}

int main() {
    try {
        py::scoped_interpreter guard{}; 

        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("append")("."); 

        py::module_ generator = py::module_::import("generator");

        // .cast<py::object>(), чтобы превратить accessor в реальный объект
        py::object generate_func = generator.attr("generate_and_solve").cast<py::object>();

        std::vector<int> test_sizes = {5, 50, 100};

        for(int size : test_sizes) {
            run_test(generate_func, size);
        }

    } catch (const std::exception& e) {
        std::cerr << "\nERROR!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
