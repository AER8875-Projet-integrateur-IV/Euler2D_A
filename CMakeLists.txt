cmake_minimum_required(VERSION 3.10)

project(Euler2D_A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

# Build tests
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    # include(CTest)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()