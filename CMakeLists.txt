cmake_minimum_required(VERSION 3.10)

project(Euler2D_A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

message(STATUS "BIN_DIR=" ${CMAKE_BINARY_DIR})

# add_compile_options_config(<CONFIG> <option> ...)
function(add_compile_options_config CONFIG)
    foreach(opt ${ARGN})
        add_compile_options("$<$<CONFIG:${CONFIG}>:${opt}>")
    endforeach()
endfunction()

add_compile_options_config(RELEASE "-W" "-Wall" "-Ofast" "-pedantic" "-fopenmp" "-I${CMAKE_CURRENT_SOURCE_DIR}/src")
add_compile_options_config(DEBUG "-W" "-Wall" "-O0" "-g3" "-pedantic" "-fopenmp" "-I${CMAKE_CURRENT_SOURCE_DIR}/src")

# Build tests
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    # include(CTest)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()