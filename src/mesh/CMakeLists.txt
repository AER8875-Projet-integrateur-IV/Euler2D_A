add_library(Mesh STATIC Mesh.cpp Marker.cpp MarkerContainer.cpp)
target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Mesh Logger)

add_subdirectory(generator)
add_subdirectory(metrics)

