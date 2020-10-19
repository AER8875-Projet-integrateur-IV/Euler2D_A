add_library(Mesh STATIC Mesh.cpp)
target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(generator)