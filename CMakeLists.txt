cmake_minimum_required(VERSION 3.26)
project(MathModule)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Add GoogleTest directory
find_package(GTest REQUIRED)

# Include source headers
include_directories(big/)

# Add your source files
file(GLOB_RECURSE SOURCES "big/*.cpp")
add_executable(MathModule_run ${SOURCES})

# Add your test files
add_executable(MathModule_test 	big/natural/natural.hpp 
				big/natural/natural.cpp 
				big/integer/integer.hpp 
				big/integer/integer.cpp 
				test/TestNatural.cpp 
				test/TestInteger.cpp 
				test/main.cpp)

# Link GoogleTest to the test executable
target_link_libraries(MathModule_test PRIVATE gtest GTest::GTest GTest::Main)

# Include GoogleTest's CMake functions
include(GoogleTest)

gtest_discover_tests(MathModule_test)
