
add_library(Metrics STATIC
            MetricsGenerator.cpp)
target_include_directories(Metrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

