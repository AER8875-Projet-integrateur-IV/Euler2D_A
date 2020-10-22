
add_library(MeshGeneration STATIC
            MeshReader.cpp MeshReaderSU2.cpp Parser.cpp MeshGenerator.cpp)
target_include_directories(MeshGeneration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MeshGeneration Mesh)
