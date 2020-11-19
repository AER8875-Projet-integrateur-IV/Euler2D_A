add_library(PostProcessing STATIC
            TecplotWriter.cpp Coefficient.cpp)
target_include_directories(PostProcessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

