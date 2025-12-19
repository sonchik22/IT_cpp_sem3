cmake_minimum_required(VERSION 3.14)
project(MatrixPyTest)

set(CMAKE_CXX_STANDARD 17)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.11.1
)
set(PYBIND11_TEST OFF)
FetchContent_MakeAvailable(pybind11)

add_executable(main main_matrix.cpp)

# Ссылка на pybind11
target_link_libraries(main PRIVATE pybind11::embed)

# Копирование generator.py в папку сборки
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/generator.py"
        "$<TARGET_FILE_DIR:main>/generator.py"
)
