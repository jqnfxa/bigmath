cmake_minimum_required(VERSION 3.26)
project(MathModule)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Enable testing
enable_testing()

# Add GoogleTest directory
find_package(GTest REQUIRED)

# Include source headers
include_directories(big/)

# source files
file(GLOB_RECURSE SOURCES "big/*.cpp")
add_executable(bignum ${SOURCES} main.cpp)

# test files
add_executable(bignum_test big/natural/natural.cpp
                           big/integer/integer.cpp
			   big/rational/rational.cpp
                           big/polynomial/polynomial.cpp
                           test/TestNatural.cpp
                           test/TestInteger.cpp
                           test/TestRational.cpp
                           test/TestPolynomial.cpp
                           test/TestAlgorithms.cpp
                           test/TestExpressionParser.cpp
                           test/main.cpp)

# Link GoogleTest to the test executable
target_link_libraries(bignum_test PRIVATE gtest GTest::GTest GTest::Main)

# Include GoogleTest's CMake functions
include(GoogleTest)

gtest_discover_tests(bignum_test)
