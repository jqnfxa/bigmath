cmake_minimum_required(VERSION 3.26)
project(MathModule)

set(CMAKE_CXX_STANDARD 20)

add_executable(MathModule main.cpp
        big/natural/natural.cpp
        big/natural/natural.hpp)
