add_library(MeshGeneration 
            MeshReader.cpp MeshReaderSU2.cpp Parser.cpp)
target_include_directories(MeshGeneration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

