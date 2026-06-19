cmake_minimum_required(VERSION 3.20)

project(lab3_matrix_optimized)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab3
        src/main.cpp
        src/tests.cpp
        src/ui.cpp
)
