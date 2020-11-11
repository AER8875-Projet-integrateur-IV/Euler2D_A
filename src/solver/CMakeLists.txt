add_library(Solver
            Solver.cpp Residual.cpp)
target_link_libraries(Solver Logger Mesh InputParser)
target_include_directories(Solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

