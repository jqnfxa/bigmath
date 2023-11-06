cmake_minimum_required(VERSION 3.26)
project(MathModule)

set(CMAKE_CXX_STANDARD 20)

add_executable(MathModule main.cpp
        big/natural/natural.cpp
        big/natural/natural.hpp)

add_executable(
	test
	tst/main.cpp
	tst/TestNatural.cpp
	tst/TestInteger.cpp
	big/integer/integer.cpp
	big/natural/natural.cpp
)

find_package(GTest)
target_link_libraries(test GTest)
